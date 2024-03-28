/** 
  @file cshpom.CPP

  @author KaiYang Zhu
  @date 21-Dec-2023

*/

#include <math.h>
#include <vector>
#include <numeric>
#include <iomanip>
#include <time.h>
#include "ga/GASimpleGA.h"
#include "ga/GABin2DecGenome.h"
#include "ga/setSWMMParam.h"
#include "ga/ioSWMM.h"
#include "./swmm522_engine/swmm5.h"
#include "./swmm522_engine/headers.h"


//=============================================================================
// Global variables
//=============================================================================
const char *inputFile;
const char *reportFile;
const char *binaryFile;
const char *WLS_File;
const char *updatedSWMMINPFile = "../user_data/update_SWMM_INP.inp";

//=============================================================================
// Function declarations
//=============================================================================
float Objective(GAGenome& g);

//=============================================================================

int main(int argc, char *argv[])
//
//  Input:   argc = number of command line arguments
//           argv = array of command line arguments
//  Output:  returns error status
//  Purpose: runs the command line version of Cloud SWMM Hydro-Parameters Optimization Model
//
//  Command line is: cshpom arg_1 arg_2 arg_3 arg_4 arg_5
//  Where arg_1 represents the user inputs -o, it will determine whether to run the GA to optimize the SWMM parameters or not depend on the arg_5.
//  arg_2 = name of input file
//  arg_3 = name of report file
//  arg_4 = name of output file if saved (or blank if not saved).
//  arg_5 = name of WLS file  (WLS: Water Level Station)
//
{

    char blank[] = "";
    int  version, vMajor, vMinor, vRelease;
    char errMsg[128];
    int  msgLen = 127;
    time_t start, end;
    double runTime;

    version = swmm_getVersion();
    vMajor = version / 10000;
    vMinor = (version - 10000 * vMajor) / 1000;
    vRelease = (version - 10000 * vMajor - 1000 * vMinor);
    start = time(0);

    // Read the parameter file. Creat a new paramFile if doesn't exist.
    

    // --- check for proper number of command line arguments
    if (argc == 1 || ((argc > 1 && argc < 6) && std::string(argv[1]) == "-o"))
    {
        std::cout << "\nNot Enough Arguments (See Help -h)" << "\n\n";
    }
    else if (argc == 2)
    {
        if (std::string(argv[1]) == "-h")
        {
            // Help
            std::cout << "\n\n" << "CLOUD SWMM HYDRO-PARAMETER OPTIMIZATION MODEL (CSHPOM) HELP" << "\n\n";
            std::cout << "COMMANDS:" << "\n";
            std::cout << "\t" << "-h       CSHPOM Help" << "\n";
            std::cout << "\n" << "RUNNING A SIMULATION:" << "\n";
            std::cout << "\t" << "cshpom <input file> <report file> <optional output file>" << "\n\n";
            std::cout << "\n" << "RUNNING A SIMULATION WITH OPTIMIZING HYDRO-PARAMETERS:" << "\n";
            std::cout << "\t" << "cshpom <-o> <input file> <report file> <optional output file> <WLS file>" << "\n\n";
        }
        else
        {
            std::cout << "Unknown Argument (See Help -h)" << "\n\n";
        }

    }
    else if (argc == 4)
    {
        if (strcmp(argv[1], "*.inp") && strcmp(argv[2], "*.rpt") && strcmp(argv[3], "*.out"))
        {
            // --- extract file names from command line arguments
            inputFile = argv[1];
            reportFile = argv[2];
            if (argc > 3) binaryFile = argv[3];
            else          binaryFile = blank;
            printf("\n... EPA SWMM %d.%d (Build %d.%d.%0d)\n", vMajor, vMinor,
                vMajor, vMinor, vRelease);

            // --- run SWMM
            swmm_run(inputFile, reportFile, binaryFile);

            // Display closing status on console
            runTime = difftime(time(0), start);
            printf("\n\n... EPA SWMM completed in %.2f seconds.", runTime);
            if      ( swmm_getError(errMsg, msgLen) > 0 ) printf(" There are errors.\n");
            else if ( swmm_getWarnings() > 0 ) printf(" There are warnings.\n");
            else printf("\n");
        }
        else
        {
            std::cout << "Unknown Argument (See Help -h)" << "\n\n";
        }
    }
    else if (argc == 6)
    {
        inputFile = argv[2];
        reportFile = argv[3];
        binaryFile = argv[4];
        WLS_File = argv[5];

        readSWMMINPFile(inputFile);
        read_WLS_File(WLS_File);

        // Declare variables for the GA parameters and set them to some default values.

            unsigned int seed  = 42;      // Seed
            int popsize  = 1;      // Population
            int ngen     = 1;      // Generation
            float pmut   = 0.001;  // Mutation
            float pcross = 0.6;    // Crossover

        /*  Value of the parameters => 
            {
                stored_subcatchFracImperN => 0-100,
                stored_subcatchImperv01N => 0.011-0.025, 
                stored_subcatchPerv01N => 0.02-0.08, 
                stored_subcatchImperv1DStore => 0-5, 
                stored_subcatchPerv1DStore => 0-5, 
                stored_subcatchImperv0FArea => 0-100, 
                stored_subcatchF0 => 25.4-152.4, 
                stored_subcatchFMin => 3.81-7.62, 
                stored_subcatchDecay => 0.67-49, 
                stored_subcatchRegen => 2-14, 
                stored_LinkManning => 0.01-0.02, 
                stored_LinkCLossInlet => 0.2-0.9, 
            }
        */

            float min[] = {0,   0.011, 0.02, 0, 0, 0,   25.4,  3.81, 0.67, 2, 0.01, 0.2};
            float max[] = {100, 0.025, 0.08, 5, 5, 100, 152.4, 7.62, 49, 14, 0.02, 0.9};

        // Create a phenotype then fill it with the phenotypes we will need to map to
        // the values we read from the file.  The arguments to the add() method of a
        // Bin2Dec phenotype are (1) number of bits, (2) min value, and (3) max value.
        // The phenotype maps a floating-point number onto the number of bits that
        // you designate.  Here we just make everything use 16 bits and use the max and
        // min that were used to generate the target values.  You can experiment with
        // the number of bits and max/min values in order to make the GA work better
        // or worse.

            GABin2DecPhenotype map;

            for(int paramIndex = 0; paramIndex < 12; paramIndex++)
            {
                if(paramIndex < 10)
                {
                    for(int Count = 0; Count < SubcatchNum; Count++)
                    {
                        map.add(16, min[paramIndex], max[paramIndex]);
                        
                    }
                }
                else
                {
                    for(int Count = 0; Count < LinkNum; Count++)
                    {
                        map.add(16, min[paramIndex], max[paramIndex]);
                    }
                }
            }

        // Create the template genome using the phenotype map we just made.  The
        // GA will use this genome to clone the population that it uses to do the
        // evolution.  We pass the objective function to create the genome.  We
        // also use the user data function in the genome to keep track of our
        // target values.
            
            GABin2DecGenome genome(map, Objective);

        // Now create the GA using the genome, set the parameters, and run it.

            GASimpleGA ga(genome);

            ga.populationSize(popsize);
            ga.nGenerations(ngen);
            ga.pMutation(pmut);
            ga.pCrossover(pcross);
            ga.scoreFilename("bog.dat");  //ga.scoreFilename(): Specify the name of the file to which the scores should be recorded.
            ga.flushFrequency(1);	// stored scores to disk every generation
            ga.evolve(seed);
            genome = ga.statistics().bestIndividual();

            best_swmmParam.stored_subcatchFracImperN = new double[SubcatchNum];
            best_swmmParam.stored_subcatchImperv01N = new double[SubcatchNum];
            best_swmmParam.stored_subcatchPerv01N = new double[SubcatchNum];
            best_swmmParam.stored_subcatchImperv1DStore = new double[SubcatchNum];
            best_swmmParam.stored_subcatchPerv1DStore = new double[SubcatchNum];
            best_swmmParam.stored_subcatchImperv0FArea = new double[SubcatchNum];
            best_swmmParam.stored_subcatchF0 = new double[SubcatchNum];
            best_swmmParam.stored_subcatchFMin = new double[SubcatchNum];
            best_swmmParam.stored_subcatchDecay = new double[SubcatchNum];
            best_swmmParam.stored_subcatchRegen = new double[SubcatchNum];
            best_swmmParam.stored_LinkManning = new double[LinkNum];
            best_swmmParam.stored_LinkCLossInlet = new double[LinkNum];


        // Store the genome to best_swmmParam.
            for (int i = 0; i < (SubcatchNum * 10 + LinkNum * 2); i++)
            {      
                if(i  < SubcatchNum * 1)
                {
                    best_swmmParam.stored_subcatchFracImperN[i] = genome.phenotype(i);
                }        
                else if(i  < SubcatchNum * 2)
                {
                    best_swmmParam.stored_subcatchImperv01N[i - SubcatchNum * 1] = genome.phenotype(i);
                }        
                else if(i  < SubcatchNum * 3)
                {
                    best_swmmParam.stored_subcatchPerv01N[i - SubcatchNum * 2] = genome.phenotype(i);
                }        
                else if(i  < SubcatchNum * 4)
                {
                    best_swmmParam.stored_subcatchImperv1DStore[i - SubcatchNum * 3] = genome.phenotype(i);
                }        
                else if(i  < SubcatchNum * 5)
                {
                    best_swmmParam.stored_subcatchPerv1DStore[i - SubcatchNum * 4] = genome.phenotype(i);
                }        
                else if(i  < SubcatchNum * 6)
                {
                    best_swmmParam.stored_subcatchImperv0FArea[i - SubcatchNum * 5] = genome.phenotype(i);
                }        
                else if(i  < SubcatchNum * 7)
                {
                    best_swmmParam.stored_subcatchF0[i - SubcatchNum * 6] = genome.phenotype(i);
                }        
                else if(i  < SubcatchNum * 8)
                {
                    best_swmmParam.stored_subcatchFMin[i - SubcatchNum * 7] = genome.phenotype(i);
                }        
                else if(i  < SubcatchNum * 9)
                {
                    best_swmmParam.stored_subcatchDecay[i - SubcatchNum * 8] = genome.phenotype(i);
                }        
                else if(i  < SubcatchNum * 10)
                {
                    best_swmmParam.stored_subcatchRegen[i - SubcatchNum * 9] = genome.phenotype(i);
                }               
                else if(i < SubcatchNum * 10 + LinkNum * 1)
                {
                    best_swmmParam.stored_LinkManning[i - (SubcatchNum * 10)] = genome.phenotype(i);
                }        
                else
                {
                    best_swmmParam.stored_LinkCLossInlet[i - (SubcatchNum * 10 + LinkNum * 1)] = genome.phenotype(i);
                }                    
            }

        // Update the SWMM INP file
            updateSWMMINP(updatedSWMMINPFile);

            delete[] best_swmmParam.stored_subcatchFracImperN;
            delete[] best_swmmParam.stored_subcatchImperv01N;
            delete[] best_swmmParam.stored_subcatchPerv01N;
            delete[] best_swmmParam.stored_subcatchImperv1DStore;
            delete[] best_swmmParam.stored_subcatchPerv1DStore;
            delete[] best_swmmParam.stored_subcatchImperv0FArea;
            delete[] best_swmmParam.stored_subcatchF0;
            delete[] best_swmmParam.stored_subcatchFMin;
            delete[] best_swmmParam.stored_subcatchDecay;
            delete[] best_swmmParam.stored_subcatchRegen;
            delete[] best_swmmParam.stored_LinkManning;
            delete[] best_swmmParam.stored_LinkCLossInlet;

        // We could print out the genome directly, like this:
        // cout << genome << "\n";

            std::cout << "best of generation data are in 'bog.dat'\n";

            std::cout << "CSHPOM COMPUTE FINISH!!!" << std::endl;
            
        // Get the program execution time
            end = time(NULL);
            double runtime = difftime(end, start);
            std:: cout << "Program execution time = " << runtime << " (seconds)" <<"\n";
    }

    return 0;
}

//=============================================================================
// Objective Function definitions
//=============================================================================

float Objective(GAGenome& g)
{
    double elapsedTime;
    float MassBalErr = massbal_getStepFlowError();
    float SE = 0;
    float MSE;

    random_swmmParam.stored_subcatchFracImperN = new double[SubcatchNum];
    random_swmmParam.stored_subcatchImperv01N = new double[SubcatchNum];
    random_swmmParam.stored_subcatchPerv01N = new double[SubcatchNum];
    random_swmmParam.stored_subcatchImperv1DStore = new double[SubcatchNum];
    random_swmmParam.stored_subcatchPerv1DStore = new double[SubcatchNum];
    random_swmmParam.stored_subcatchImperv0FArea = new double[SubcatchNum];
    random_swmmParam.stored_subcatchF0 = new double[SubcatchNum];
    random_swmmParam.stored_subcatchFMin = new double[SubcatchNum];
    random_swmmParam.stored_subcatchDecay = new double[SubcatchNum];
    random_swmmParam.stored_subcatchRegen = new double[SubcatchNum];
    random_swmmParam.stored_LinkManning = new double[LinkNum];
    random_swmmParam.stored_LinkCLossInlet = new double[LinkNum];

    GABin2DecGenome & genome = (GABin2DecGenome &)g;

// Store the genome to random_swmmParam.
    for (int i = 0; i < (SubcatchNum * 10 + LinkNum * 2); i++)
    {      
        if(i  < SubcatchNum * 1)
        {
            random_swmmParam.stored_subcatchFracImperN[i] = genome.phenotype(i);
        }        
        else if(i  < SubcatchNum * 2)
        {
            random_swmmParam.stored_subcatchImperv01N[i - SubcatchNum * 1] = genome.phenotype(i);
        }        
        else if(i  < SubcatchNum * 3)
        {
            random_swmmParam.stored_subcatchPerv01N[i - SubcatchNum * 2] = genome.phenotype(i);
        }        
        else if(i  < SubcatchNum * 4)
        {
            random_swmmParam.stored_subcatchImperv1DStore[i - SubcatchNum * 3] = genome.phenotype(i);
        }        
        else if(i  < SubcatchNum * 5)
        {
            random_swmmParam.stored_subcatchPerv1DStore[i - SubcatchNum * 4] = genome.phenotype(i);
        }
        else if(i  < SubcatchNum * 6)
        {
            random_swmmParam.stored_subcatchImperv0FArea[i - SubcatchNum * 5] = genome.phenotype(i);
        }
        else if(i  < SubcatchNum * 7)
        {
            random_swmmParam.stored_subcatchF0[i - SubcatchNum * 6] = genome.phenotype(i);
        }        
        else if(i  < SubcatchNum * 8)
        {
            random_swmmParam.stored_subcatchFMin[i - SubcatchNum * 7] = genome.phenotype(i);
        }        
        else if(i  < SubcatchNum * 9)
        {
            random_swmmParam.stored_subcatchDecay[i - SubcatchNum * 8] = genome.phenotype(i);
        }
        else if(i  < SubcatchNum * 10)
        {
            random_swmmParam.stored_subcatchRegen[i - SubcatchNum * 9] = genome.phenotype(i);

        }
        else if(i < (SubcatchNum * 10 + LinkNum * 1))
        {
            random_swmmParam.stored_LinkManning[i - SubcatchNum * 10] = genome.phenotype(i);
        }
        else
        {
            random_swmmParam.stored_LinkCLossInlet[i - (SubcatchNum * 10 + LinkNum * 1)] = genome.phenotype(i);
        }
    }

// --- open the files & read input data
    swmm_open(inputFile, reportFile, binaryFile);

// --- initialize values
    swmm_start(TRUE);

// --- set subcatchment, infiltration, link parameter
    setSWMMParam();

// --- execute each time step until elapsed time is re-set to 0
    do {
        swmm_step(&elapsedTime);
    } while (elapsedTime > 0);

// --- clean up
    swmm_end();

// --- save simlated node head to vector
    for(int stationIndex = 0; stationIndex < WLS_stationData.size(); stationIndex++)
    {
        auto stationdata = WLS_stationData[stationIndex];
        const char* stationdataCstr = stationdata.c_str();

        for(int time = 1; time <= I_timeseriesData.size(); time++)
        {
            WLS_simNodeHead.push_back(swmm_getSavedValue(swmm_NODE_HEAD, swmm_getIndex(swmm_NODE, stationdataCstr), time));
        }
    }

// --- set the condition if MassBalErr less or equal than the condition
    if(std::fabs(MassBalErr) <= 0.45)
    {
        for(int count = 0; count < WLS_nodeHeadData.size(); count++)//swmm_getValue(swmm_TOTALSTEPS, 0)
        {
            const WLS_NodeHead& WLS_realNodeHead = WLS_nodeHeadData[count];
            SE += pow((WLS_simNodeHead[count] - WLS_realNodeHead.value), 2);
        }

        MSE = SE / WLS_nodeHeadData.size();

        std::cout << "MassBalErr: " << MassBalErr << "\t" << "MSE: " << MSE <<"\n";

    }
    else
    {
        MSE = 9999;    // 9999 means the MSE cannot be recorded with the population.
    }

    WLS_simNodeHead.clear();

    delete[] random_swmmParam.stored_subcatchFracImperN;
    delete[] random_swmmParam.stored_subcatchImperv01N;
    delete[] random_swmmParam.stored_subcatchPerv01N;
    delete[] random_swmmParam.stored_subcatchImperv1DStore;
    delete[] random_swmmParam.stored_subcatchPerv1DStore;
    delete[] random_swmmParam.stored_subcatchImperv0FArea;
    delete[] random_swmmParam.stored_subcatchF0;
    delete[] random_swmmParam.stored_subcatchFMin;
    delete[] random_swmmParam.stored_subcatchDecay;
    delete[] random_swmmParam.stored_subcatchRegen;
    delete[] random_swmmParam.stored_LinkManning;
    delete[] random_swmmParam.stored_LinkCLossInlet;

// --- close the system
    swmm_close();

    return -MSE;
}

//=============================================================================