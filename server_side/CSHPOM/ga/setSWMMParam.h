#ifndef SETSWMMPARAM_H
#define SETSWMMPARAM_H

#include "../swmm522_engine/headers.h"
#include "../swmm522_engine/swmm5.h"
#include "./ioSWMM.h"
#include <malloc.h>

extern double** sortedRandomSubcatchParam;
extern double** sortedRandomInfilParam;
extern double* randomSubcatchParamArr;
extern double* randomInfilParamArr;
extern double** randomLinkParamArr;


struct random_swmmParamStruct
{
    double* stored_subcatchFracImperN;
    double* stored_subcatchImperv01N;
    double* stored_subcatchPerv01N;
    double* stored_subcatchImperv1DStore;
    double* stored_subcatchPerv1DStore;
    double* stored_subcatchImperv0FArea;
    double* stored_subcatchF0;
    double* stored_subcatchFMin;
    double* stored_subcatchDecay;
    double* stored_subcatchRegen;
    double* stored_LinkManning;
    double* stored_LinkCLossInlet;
};

struct best_swmmParamStruct
{
    double* stored_subcatchFracImperN;
    double* stored_subcatchImperv01N;
    double* stored_subcatchPerv01N;
    double* stored_subcatchImperv1DStore;
    double* stored_subcatchPerv1DStore;
    double* stored_subcatchImperv0FArea;
    double* stored_subcatchF0;
    double* stored_subcatchFMin;
    double* stored_subcatchDecay;
    double* stored_subcatchRegen;
    double* stored_LinkManning;
    double* stored_LinkCLossInlet;
};

extern double** sortedRandomSubcatchParam;
extern double** sortedRandomInfilParam;
extern double* randomSubcatchParamArr;
extern double* randomInfilParamArr;
extern double** randomLinkParamArr;
extern random_swmmParamStruct random_swmmParam;
extern best_swmmParamStruct best_swmmParam;
extern void setSWMMParam();

#endif //SETSWMMPARAM_H