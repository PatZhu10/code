#include "setSWMMParam.h"

random_swmmParamStruct random_swmmParam;
best_swmmParamStruct best_swmmParam;

double** sortedRandomSubcatchParam;
double** sortedRandomInfilParam;
double* randomSubcatchParamArr;
double* randomInfilParamArr;
double** randomLinkParamArr;

//=============================================================================
// Set SWMM parameter
//=============================================================================
void setSWMMParam()
{
    sortedRandomSubcatchParam = new double*[SubcatchNum];
    sortedRandomInfilParam = new double*[SubcatchNum];
    for (int row = 0; row < SubcatchNum; row++)
    {
        sortedRandomSubcatchParam[row] = new double[6];
        sortedRandomInfilParam[row] = new double[4];
    }

//  Set the subcatchment parameters //
//-----------------------------------------------------------------------------
//  subcatch_setParams(int i, double x[])
// 
//  Input: i = subcatchment index
//         x[] = array of parameter values
// Output: none
// Purpose: sets parameters for a subcatchment.
//
// Format:
// 
//     subcatchmentID  %Imperv  N-Imperv  N-Perv  S-Imperv  S-Perv  PctZero
//                      x[0]    x[1]        x[2]    x[3]    x[4]    x[5]
//-----------------------------------------------------------------------------

// Sort the subcatchment parameters that created by genome (step 1)
    for (int col = 0; col < 5; col++)
    {
        if (col == 0)
        {
            for (int row = 0; row < (SubcatchNum); row++)
            {
                sortedRandomSubcatchParam[row][col] = random_swmmParam.stored_subcatchFracImperv[row];
            }
        }
        if (col == 1)
        {
            for (int row = 0; row < (SubcatchNum); row++)
            {
                sortedRandomSubcatchParam[row][col] = random_swmmParam.stored_subcatchImperv01N[row];
            }
        }
        if (col == 2)
        {
            for (int row = 0; row < (SubcatchNum); row++)
            {
                sortedRandomSubcatchParam[row][col] = random_swmmParam.stored_subcatchPerv01N[row];
            }
        }
        if (col == 3)
        {
            for (int row = 0; row < (SubcatchNum); row++)
            {
                sortedRandomSubcatchParam[row][col] = random_swmmParam.stored_subcatchImperv1DStore[row];
            }
        }
        if (col == 4)
        {
            for (int row = 0; row < (SubcatchNum); row++)
            {
                sortedRandomSubcatchParam[row][col] = random_swmmParam.stored_subcatchPerv1DStore[row];
            }
        }
        // if (col == 5)
        // {
        //     for (int row = 0; row < (SubcatchNum); row++)
        //     {
        //         sortedRandomSubcatchParam[row][col] = random_swmmParam.stored_subcatchImperv0FArea[row];
        //     }
        // }
    }

// Store the subcatchment parameters in new array which fit in with the format of SWMM API (step 2)
    for (int subcatchmentIndex = 0; subcatchmentIndex < SubcatchNum; subcatchmentIndex++)
    {
        randomSubcatchParamArr = new double[SubcatchNum];

        for (int col = 0; col< 5; col++)
        {
            randomSubcatchParamArr[col] = sortedRandomSubcatchParam[subcatchmentIndex][col];
        }

        subcatch_setParams(subcatchmentIndex, randomSubcatchParamArr);
        delete[] randomSubcatchParamArr;
    }

// Set the Infiltration parameters //
//-----------------------------------------------------------------------------
//  horton_setParams(THorton *infil, double p[])
//
//  Input:   infil = ptr. to Horton infiltration object
//           p[] = array of parameter values
//  Output:  returns TRUE if parameters are valid, FALSE otherwise
//  Purpose: assigns Horton infiltration parameters to a subcatchment.
//
// Format:
// 
//     subcatchmentID  F0   FMin    Decay   Regen
//                    p[0]  p[1]    p[2]    p[3]
//-----------------------------------------------------------------------------

// Sort the infiltration parameters that created by genome (step 1)

    for (int col = 0; col < 3; col++)
    {
        if (col == 0)
        {
            for (int row = 0; row < (SubcatchNum); row++)
            {
                sortedRandomInfilParam[row][col] = random_swmmParam.stored_subcatchF0[row];
            }
        }
        if (col == 1)
        {
            for (int row = 0; row < (SubcatchNum); row++)
            {
                sortedRandomInfilParam[row][col] = random_swmmParam.stored_subcatchFMin[row];
            }
        }
        if (col == 2)
        {
            for (int row = 0; row < (SubcatchNum); row++)
            {
                sortedRandomInfilParam[row][col] = random_swmmParam.stored_subcatchDecay[row];
            }
        }
        // if (col == 3)
        // {
        //     for (int row = 0; row < (SubcatchNum); row++)
        //     {
        //         sortedRandomInfilParam[row][col] = swmm_getValue(swmm_INFILTRATION_REGEN, row);
        //     }
        // }

        
    }

// Store the infiltration parameters in new array which fit in with the format of SWMM API (step 2)

    for (int subcatchmentIndex = 0; subcatchmentIndex < SubcatchNum; subcatchmentIndex++)
    {
        randomInfilParamArr = new double[SubcatchNum];
        for (int col = 0; col< 3; col++)
        {
            randomInfilParamArr[col] = sortedRandomInfilParam[subcatchmentIndex][col];

        }
            
        horton_setParams(&InfilMethod[subcatchmentIndex].horton, randomInfilParamArr);
        delete[] randomInfilParamArr;
    }

// Set the link parameters //
//-----------------------------------------------------------------------------
//  link_setParams(int j, int type, int n1, int n2, int k, double x)
//
//  Input:   j    = link index
//           type = link type code
//           n1   = index of upstream node
//           n2   = index of downstream node
//           k    = index of link's sub-type
//           x    = array of parameter values
// Output: none
// Purpose: sets parameters for a link.
//
// Format:
// 
//     linkID  Length   Roughness    Offset1   Offset2  Q0  Qlimit
//              x[0]    x[1]         x[2]       x[3]   x[4]  x[5]
//-----------------------------------------------------------------------------

// Store the link parameters in new array which fit in with the format of SWMM API
    for (int linkIndex = 0; linkIndex < LinkNum; linkIndex++)
    {
        randomLinkParamArr = new double*[LinkNum];
        for (int row = 0; row < LinkNum; row++)
        {
            randomLinkParamArr[row] = new double[6];
        }
        randomLinkParamArr[linkIndex][0] = swmm_getValue(swmm_LINK_LENGTH, linkIndex);
        randomLinkParamArr[linkIndex][1] = random_swmmParam.stored_LinkManning[linkIndex]; // Manning's N
        randomLinkParamArr[linkIndex][2] = swmm_getValue(swmm_LINK_OFFSET1, linkIndex);
        randomLinkParamArr[linkIndex][3] = swmm_getValue(swmm_LINK_OFFSET2, linkIndex);
        randomLinkParamArr[linkIndex][4] = swmm_getValue(swmm_LINK_Q0, linkIndex);
        randomLinkParamArr[linkIndex][5] = swmm_getValue(swmm_LINK_QLIMIT, linkIndex);

        link_setParams(linkIndex, CONDUIT, swmm_getValue(swmm_LINK_NODE1, linkIndex), swmm_getValue(swmm_LINK_NODE2, linkIndex), linkIndex, randomLinkParamArr[linkIndex]);

// //-----------------------------------------------------------------------------
// //  link_LossParams(int j, int type, int k, double x)
// //
// //  Input:   j    = link index
// //           type = link type code
// //           k    = index of link's sub-type
// //           x    = parameter values
// // Output: none
// // Purpose: sets parameters for a link loss.
// //-----------------------------------------------------------------------------
//         link_setLossParams(linkIndex, CONDUIT, linkIndex, random_swmmParam.stored_LinkCLossInlet[linkIndex]);

        for (int row = 0; row < LinkNum; row++)
        {
            delete[] randomLinkParamArr[row];
        }
        delete[] randomLinkParamArr;
        
    }
    
    for (int row = 0; row < SubcatchNum; row++)
    {
        delete[] sortedRandomSubcatchParam[row];
        delete[] sortedRandomInfilParam[row];
    }

    delete[] sortedRandomSubcatchParam;
    delete[] sortedRandomInfilParam;
}