#include "system.h"

// private data for the epochs

static const epoch phugpa_epoch= // we take -1000 as a reference
{
    -1000,
    7,
    6,
    61,
    10,
    26,
    5,
    1355847,
    93,
    6663418,
    2080,
    277,
    4,
    511,
    2995,
    48,
    { 5, 35, 36, 4, 160, 0 },
    { 26, 45, 53, 4, 26, 0 },
};

static const epoch tsurphu_epoch= // we take -1000 as a reference
{
    -1000,
    11,
    6,
    75,
    50,
    66,
    5,
    1355847,
    93,
    83343,
    1977,
    268,
    3,
    512,
    2988,
    0, // used only for phugpa
    { 5, 38, 22, 3, 701, 0 },
    { 0, 7, 25, 0, 30, 0 },
};
