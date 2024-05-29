
#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once
#include <vector>
#include <string>
#include <assert.h>
#include <limits.h>
#include <array>


/********************* Macros *********************/
#define _aligned_malloc(size,alignment) aligned_alloc(alignment,size)
#define _aligned_free free
#define getrandom(min, max) ((rand()%(int)(((max) + 1)-(min)))+ (min))
#define floatToMyType(a) ((myType)(int)floor(a * (1 << FLOAT_PRECISION)))


/********************* AES and other globals *********************/
#define LOG_DEBUG false
#define LOG_DEBUG_NETWORK false
#define FUNCTION_TIME true
#define RANDOM_COMPUTE 256	//Size of buffer for random elements
#define STRING_BUFFER_SIZE 256
#define PARALLEL false
#define NO_CORES 8
//#define MSS_BOUND 1048576 //2^20
#define MSS_BOUND 1099511627776 //2^40 



/********************* MPC globals *********************/
#define NUM_OF_PARTIES 2
#define PARTY_A 0
#define PARTY_B 1
#define PARTY_C 2
#define USING_EIGEN true
#define PRIME_NUMBER 67
#define FLOAT_PRECISION 13
#define PRECISE_DIVISION false


/********************* Neural Network globals *********************/
//Batch size has to be a power of two
#define LOG_MINI_BATCH 0
#define MINI_BATCH_SIZE (1 << LOG_MINI_BATCH)
#define LOG_LEARNING_RATE 5
#define LEARNING_RATE (1 << (FLOAT_PRECISION - LOG_LEARNING_RATE))
#define NO_OF_EPOCHS 1.5
#define NUM_ITERATIONS 1
// #define NUM_ITERATIONS ((int) (NO_OF_EPOCHS * TRAINING_DATA_SIZE/MINI_BATCH_SIZE))



/********************* Typedefs and others *********************/
typedef uint64_t myType;
typedef uint8_t smallType;
//typedef std::pair<myType, myType> RSSMyType;
//typedef std::pair<smallType, smallType> RSSSmallType;
//typedef std::vector<RSSMyType> RSSVectorMyType;
//typedef std::vector<RSSSmallType> RSSVectorSmallType;

//MSS
typedef std::vector<myType> ASSVectorMyType;
typedef std::vector<smallType> ASSVectorSmallType;

// define the data structure for Meteor, this is for Arithmetic Sharing.
//typedef std::pair<myType, RSSMyType> METype;
//typedef std::vector<METype> MEVectorType;
//typedef std::pair<smallType, RSSSmallType> MESmallType;
//typedef std::vector<MESmallType> MEVectorSmallType;

//MSS
typedef std::pair<myType, myType> MSSType;
typedef std::vector<MSSType> MSSVectorType;
typedef std::pair<smallType, smallType> MSSSmallType;
typedef std::vector<MSSSmallType> MSSVectorSmallType;

const int BIT_SIZE = (sizeof(myType) * CHAR_BIT);
const myType LARGEST_NEG = ((myType)1 << (BIT_SIZE - 1));
const myType MINUS_ONE = (myType)-1;
const smallType BOUNDARY = (256/PRIME_NUMBER) * PRIME_NUMBER;



#endif
