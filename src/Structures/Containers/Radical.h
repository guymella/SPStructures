#pragma once
//------------------------------------------------------------------------------
/**
    @class Structures::Block
    @ingroup Block
    @brief inline Block of dynamic Memory, Self Allocating on heap
*/
#include "Interfaces/iBlock.h"
#include "Table.h"

class keyBuilder : public BlockDV{
public:
    keyBuilder() {};
    keyBuilder(const char* key) {
        Add(key);
    }
    void Add(const char* str) { Add((uint8_t* )str, std::strlen(str)); }
    void Add(const iBlockD& toAdd) { Add(toAdd.begin().Ptr(), toAdd.Size()); }
    void Add(const uint8_t* str, size_t numElements) {
        size_t s = Size();
        ReserveBack(numElements);
        std::memcpy(begin(s).Ptr(), str, numElements);
    }
};


struct keyCompare {
    keyCompare(uint8_t* lhs, uint8_t* rhs, size_t lhsSize, size_t rhsSize);
    size_t commonPrefix = 0;
    size_t lhsPostfix = 0;
    size_t rhsPostfix = 0;
    bool lhsLesser = false;
    bool Equal() { return !(lhsPostfix || rhsPostfix); }

};

keyCompare::keyCompare(uint8_t* lhs, uint8_t* rhs, size_t lhsSize, size_t rhsSize)
{
    size_t compSize = (lhsSize < rhsSize) ? lhsSize : rhsSize;

    while (*lhs == *rhs && commonPrefix < compSize)
        commonPrefix++, lhs++, rhs++;

    lhsPostfix = lhsSize - commonPrefix;
    rhsPostfix = rhsSize - commonPrefix;
    lhsLesser = *(lhs) < *(rhs);
}

class RadAdapter {
public:
    RadAdapter() {};
    RadAdapter(iTableD<>* allocator) : Allocator(allocator) {
        HeadBlockID = Allocator->Size();
        Allocator->PushBack();
        //Block(HeadBlockID).begin()[0]= 0;
    }
    RadAdapter(iTableD<>* allocator, size_t blockID) : Allocator(allocator), HeadBlockID(blockID) {};
    virtual iBlockD* Exists(iBlockD& checkKey);
    //virtual const iBlockD* Exists(iBlockD& key) const;
    virtual iBlockD& GetOrCreate(iBlockD& key);
    virtual iBlockD& operator[](iBlockD& key) { return GetOrCreate(key); }
protected:
    iBlockD& PushDownPostfix(size_t blockID, size_t keyIndex, size_t prefixLen);
    iBlockD& InsertKey(iBlockD& block, size_t keyIndex, uint8_t* KeyItr, size_t keyLen) ;
    iBlockD& InsertValue(size_t blockID, size_t keyIndex);
    //virtual RadAdapter& GetChildNode(size_t index);
    //uint8_t KeyCount() { return Block().begin()[0]; }
    inline iBlockD& Block(size_t BlockID) { return Allocator->GetBlock(BlockID); }
    size_t HeadBlockID = 0;
    iTableD<>* Allocator = 0;
};

iBlockD* RadAdapter::Exists(iBlockD& checkKey)
{
    uint8_t* ktr = checkKey.begin().Ptr();
    uint8_t* ktrEnd = checkKey.end().Ptr();
    size_t bID = HeadBlockID;
    if (Block(bID).Empty())
        return nullptr;
    while (true) {
        uint8_t* itr = Block(bID).begin().Ptr();
        if (!itr)
            return nullptr;
        uint8_t keyCount = *itr;
        if (!keyCount) //does not exist
            return nullptr;
        itr++;        
        for (uint8_t i = 0; i < keyCount; i++) {
            size_t keySize = *(size_t*)itr - (2 * sizeof(size_t));
            itr += sizeof(size_t);
            if (*itr <= *ktr) { // checkKey
                keyCompare cmp(itr, ktr, keySize, ktrEnd -ktr);
                itr += keySize;
                if (cmp.Equal()) {                    
                    if (*(size_t*)itr)
                        return &Block(*(size_t*)(itr)); // value BlockID is first
                    return nullptr;//no value
                }
                itr += sizeof(size_t);
                if (!cmp.lhsPostfix) {//partial match = recurse
                    if (!*(size_t*)itr) //no children
                        return nullptr;
                    bID = *(size_t*)(itr); //childNode BlockID is second
                    break;
                }
                return nullptr;
            }
            itr += keySize + (2 * sizeof(size_t)); // next key
        }
    }
    
    return nullptr;
}

inline iBlockD& RadAdapter::GetOrCreate(iBlockD& checkKey)
{
    uint8_t* ktr = checkKey.begin().Ptr();
    uint8_t* ktrEnd = checkKey.end().Ptr();
    size_t bID = HeadBlockID;
    while (true) {
        uint8_t* itr = Block(bID).begin().Ptr();
        if (!itr) //does not exist
            return InsertKey(Block(bID), 0, ktr, ktrEnd - ktr);
        uint8_t keyCount = *itr;
        if (!keyCount) //does not exist
            return InsertKey(Block(bID), 0, ktr, ktrEnd - ktr);
        itr++;
        for (uint8_t i = 0; i < keyCount; i++) {
            size_t keySize = (*(size_t*)itr)-(2 * sizeof(size_t));
            itr += sizeof(size_t);
            if (*itr >= *ktr) { // checkKey
                keyCompare cmp(itr, ktr, keySize, ktrEnd - ktr);
                if (!cmp.commonPrefix) {//insert new here
                    return InsertKey(Block(bID), i, ktr, cmp.rhsPostfix);
                }                
                if (cmp.Equal())
                    return Block(*(size_t*)(itr + keySize)); // value BlockID is first
                ktr += cmp.commonPrefix; //increment key by prefix
                if (!cmp.lhsPostfix) {//partial match = recurse
                    bID = *(size_t*)(itr + keySize + sizeof(size_t)); //childNode BlockID is second
                    break;
                } else { //push down postfix   
                    iBlockD& nb = PushDownPostfix(bID, i, cmp.commonPrefix);
                    if (ktrEnd - ktr)
                        return InsertKey(nb, (cmp.lhsLesser ? 1 : 0), ktr, ktrEnd - ktr);
                    else //add value to prefix
                        return InsertValue(bID, i);
                }
            }
            itr += keySize + (2 * sizeof(size_t)); // next key
        }
        return InsertKey(Block(bID), keyCount, ktr, ktrEnd - ktr);
    }

    return Block(bID);
}

inline iBlockD& RadAdapter::PushDownPostfix(size_t BlockID, size_t keyIndex, size_t prefixLen)
{
    //get key len
    uint8_t* ins = Block(BlockID).begin().Ptr();
    ins++;
    for (size_t i = 0; i < keyIndex; i++)
        ins += *(size_t*)ins + sizeof(size_t);

    size_t keylen = *(size_t*)ins;
    //ins + sizeof(size_t) + prefixLen;

    //update childblockID and valuesBlockID
    // get new block
    size_t newBlockID = Allocator->Size();
    Allocator->PushBack(1+keylen -prefixLen + sizeof(size_t));
    iBlockD& newBlock = Block(newBlockID);
    uint8_t* cpy = newBlock.begin().Ptr();
    cpy[0]= 1; //set keycound
    cpy++;
    *(size_t*)cpy = keylen - prefixLen;//set keysize
    //copy postfix
    std::memcpy(cpy + sizeof(size_t), ins + sizeof(size_t) + prefixLen, *(size_t*)cpy);

    //remove postfix    
    std::memmove(ins + sizeof(size_t), ins + *(size_t*)ins, Block(BlockID).end().Ptr() - ins + *(size_t*)ins);
    *(size_t*)ins = prefixLen + (2 * sizeof(size_t)); //set new size

    ins += prefixLen +sizeof(size_t);
    *(size_t*)ins = 0; // set value id
    ins += sizeof(size_t);
    *(size_t*)ins = newBlockID; // set children id

    Block(BlockID).Resize(Block(BlockID).Size() - (keylen - prefixLen));//trim excess size

    return newBlock;
}

inline iBlockD& RadAdapter::InsertKey(iBlockD& block, size_t keyIndex, uint8_t* KeyItr, size_t keyLen)
{
    //shift remaining

    if (block.Empty()) {
        block.ReserveBack(1);
        block.begin()[0] = 0;
    }

    uint8_t* ins = block.begin().Ptr();
    (*ins)++; //increas keycount
    ins++;
    for (size_t i = 0; i < keyIndex; i++)
        ins += *(size_t*)ins + sizeof(size_t);

    size_t s = block.end().Ptr()-ins;
    size_t tmp = ins - block.begin().Ptr();
    block.ReserveBack(keyLen + (3 * sizeof(size_t)));
    ins = block.begin(tmp).Ptr(); //iterator invalidated
    if (s)
        std::memmove(ins + keyLen + (3 * sizeof(size_t)), ins, s);
    *(size_t*)ins = keyLen+(2*sizeof(size_t)); //set key size
    ins += sizeof(size_t);
    std::memcpy(ins, KeyItr, keyLen);
    ins += keyLen;
    *(size_t*)ins = Allocator->Size();
    Allocator->PushBack();
    ins += sizeof(size_t);
    *(size_t*)ins = 0;//no children

    return Block(Allocator->Size()-1);
}

inline iBlockD& RadAdapter::InsertValue(size_t blockID, size_t keyIndex)
{
    //get key len
    uint8_t* ins = Block(blockID).begin().Ptr();
    ins++;
    for (size_t i = 0; i < keyIndex; i++)
        ins += *(size_t*)ins + sizeof(size_t);

    ins += *(size_t*)ins - sizeof(size_t);
    *(size_t*)ins = Allocator->Size();
    Allocator->PushBack();
    return Block(*(size_t*)ins);
}
