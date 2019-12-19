//------------------------------------------------------------------------------
//  VarInt.h
//------------------------------------------------------------------------------

#include "TypeDescr.h"

#include "Structures/Containers/SparseArray.h"



class Schema {
public:

	size_t SizeOfFixed();
	size_t SizeOfFixed() const ;
	size_t GetElmSize(size_t index) const;
	size_t BlockCount(uint8_t BlockIndex) const; //returns number of elements in the sequence block
	size_t BlockCount(Types::TypeSequence start, uint8_t end) const; //returns number of elements in the sequence block
	size_t FindIndex(const iKeyString& name) const;
	size_t FindIndex(const char* name) const;
	bool AddAttribute(const iKeyString& name, Types::TypeDescr type);
	bool AddAttribute(const char* name, Types::baseTypes type);	
private:	
	size_t fixedSize = 0;
	bool extendable;
	SparseArray<size_t> index;
	Array<KeyString> labels;
	Array<Types::TypeDescr> types;
	//SparseArray<Darivation> Darivations
	SparseArray<Types::Constraint> Constraints;
	//SparseArray<Schema*> SubSchemas;
};





inline size_t Schema::SizeOfFixed()
{
	if (fixedSize < std::numeric_limits<size_t>::max())
		return fixedSize;

	fixedSize = std::as_const(*this).SizeOfFixed();

	return fixedSize;
}

inline size_t Schema::SizeOfFixed() const
{
	if (fixedSize < std::numeric_limits<size_t>::max())
		return fixedSize;

	size_t Size = 0;
	size_t fix = BlockCount(0);
	for (size_t i = 0; i < fix; i++)// fixed cols
		Size += GetElmSize(i);

	size_t nul = BlockCount(1);
	Size += nul / 8 + ((nul % 8) ? 1 : 0);//null flags

	for (size_t i = 0; i < nul; i++)//null cols
		Size += GetElmSize(i);

	size_t sps = BlockCount(2);
	Size += sps / 8 + ((sps % 8) ? 1 : 0);//sparse flags

	return Size;
}

inline size_t Schema::GetElmSize(size_t index) const
{
	if (types[index].getTypeSequence(NULL) >= Types::TypeSequence::Multi)
		return std::numeric_limits<size_t>::max(); //size unknown

	size_t size = Types::SizeOf(types[index].type);
	if (!size)
		return 0;//size is null

	if (size < std::numeric_limits<size_t>::max()) { //size is fixed
		const Types::Constraint* cst = Constraints.Exists(index);
		if (cst) {
			auto dims = cst->Contains(KeyString("dim"));
			if (dims) {
				size_t dimCount = dims->Size() / sizeof(size_t);
				for (size_t i = 0; i < dimCount; i++)
					size *= ((size_t*)dims->begin().Ptr())[i];
			}
			auto fs = cst->Contains(KeyString("FixedSize"));
			if (fs) {
					size *= *((size_t*)fs->begin().Ptr());
			}
		}
	}
	else {
		if (types[index].type == Types::baseTypes::Struct) {
			const Types::Constraint* cst = Constraints.Exists(index);
			auto sch = cst->Contains(KeyString("Schema"));
			if (sch) {
				size = ((Schema*)sch->begin().Ptr())->SizeOfFixed();

				auto dims = cst->Contains(KeyString("dim"));
				if (dims) {
					size_t dimCount = dims->Size() / sizeof(size_t);
					for (size_t i = 0; i < dimCount; i++)
						size *= ((size_t*)dims->begin().Ptr())[i];
				}
				auto fs = cst->Contains(KeyString("FixedSize"));
				if (fs) {
					size *= *((size_t*)fs->begin().Ptr());
				}
			}			
		}
	}
	return size;
}


inline size_t Schema::BlockCount(uint8_t BlockIndex) const
{
	switch (BlockIndex)
	{
	case 0: return BlockCount(Types::TypeSequence::Fixed, Types::TypeSequence::Nullable);
	case 1: return BlockCount(Types::TypeSequence::Nullable,Types::TypeSequence::Sparse);
	case 2: return BlockCount(Types::TypeSequence::Sparse, Types::TypeSequence::Multi);
	case 3: return BlockCount(Types::TypeSequence::Multi, Types::TypeSequence::Var);
	case 4: return BlockCount(Types::TypeSequence::Var, Types::TypeSequence::MultiVar);
	case 5: return BlockCount(Types::TypeSequence::MultiVar, Types::TypeSequence::Uncached);
	case 6: return BlockCount(Types::TypeSequence::Uncached, (uint8_t)index.Size());
	default:
		return 0;
	}
}

inline size_t Schema::BlockCount(Types::TypeSequence start, uint8_t end) const
{
	size_t count = 0;
	for (size_t i = start; i < end; i++) {
		const size_t* seqSize = index.Exists(i);
		count += seqSize ? *seqSize : 0;
	}
	return count;
}

inline size_t Schema::FindIndex(const iKeyString& name) const
{
	//check name
	for (size_t i = 0; i < labels.Size(); i++) {
		if (labels[i] == name)
			return i;
	}
	return std::numeric_limits<size_t>::max();
}

inline size_t Schema::FindIndex(const char* name) const
{
	return FindIndex(KeyString(name));
}

inline bool Schema::AddAttribute(const iKeyString& name, Types::TypeDescr type)
{
	//check name
	if (FindIndex(name) < labels.Size())
			return false;//attribute name already used

	//insert Sort by Seq Number
	Types::TypeSequence seq = type.getTypeSequence(NULL);
	if (seq < Types::TypeSequence::Multi) //recalculate size
		fixedSize = std::numeric_limits<size_t>::max();

	size_t seqStart = 0;
	for (size_t i = 0; i < seq; i++) {
		size_t* seqSize = index.Exists(i);
		seqStart += seqSize ? *seqSize : 0;
	}

	//sort by name
	while (seqStart < labels.Size() && labels[seqStart] < name)
		seqStart++;

	//insert
	labels.Insert(seqStart, name);
	types.Insert(seqStart, type);
	index[seq]++;

	return true;
}

inline bool Schema::AddAttribute(const char* name, Types::baseTypes type)
{
	Types::TypeDescr t;
	t.type = type;
	return AddAttribute(KeyString(name), t);
}

