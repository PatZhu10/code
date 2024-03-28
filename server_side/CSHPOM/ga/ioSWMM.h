#ifndef IOSWMM_H
#define IOSWMM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "setSWMMParam.h"
#include "../swmm522_engine/swmm5.h"
#include "../swmm522_engine/headers.h"

// =========================================================
// WLS file
// =========================================================
struct WLS_NodeHead {
    std::string name;
    std::string date;
    std::string time;
    double value;
};

extern std::string msg;    // msg = messenger
extern WLS_NodeHead WLS_nodeHead;
extern std::vector <double> WLS_simNodeHead;
extern std::vector <std::string> WLS_stationData;
extern std::vector <WLS_NodeHead> WLS_nodeHeadData;

// =========================================================
// SWMM INP file
// =========================================================
struct I_Options
{
    std::string Option;
    std::string Value;
};

struct I_Evaporation
{
    std::string Data_Source;
    std::string Parameters;
};

struct I_Raingage
{
    std::string Name;
    std::string Format;
    int Interval;
    int SCF;
    std::string Source;
};

struct I_Subarea
{
    std::string Name;
    double NImperv;
    double NPerv;
    double SImperv;
    double SPerv;
    double PctZero;
    std::string RouteTo;
    double PctRouted;
};

struct I_Infiltration
{
    std::string Name;
    double Param1;
    double Param2;
    double Param3;
    double Param4;
    double Param5;
};

struct I_Subcatchment
{
    std::string Name;
    std::string Raingage;
    std::string Outlet;
    double Area;
    double PctImperv;
    double Width;
    double Slope;
    double CurbLen;
};

struct I_Junctions
{
    std::string Name;
    double Elevation;
    double Maxdepth;
    double InitDepth;
    double SurDepth;
    double Aponded;
};

struct I_Outfalls
{
    std::string Name;
    double Elevation;
    std::string Type;
    double StageData;
    std::string Gated;
    std::string RouteTo;
};

struct I_Losses
{
    std::string Conduit;
    double Kentry;
    double Kexit;
    double Kavg;
    std::string Flap;
    double Seepage;
};

struct I_Conduit
{
    std::string Name;
    std::string FromNode;
    std::string ToNode;
    double Length;
    double Roughness;
    double InOffset;
    double OutOffset;
    double InitFlow;
    double MaxFlow;
};

struct I_Pumps
{
    std::string Name;
    std::string FromNode;
    std::string ToNode;
    std::string PumpCurve;
    std::string Status;
    double Startup;
    double Shutoff;
};

struct I_Xsections
{
    std::string Link;
    std::string Shape;
    double Geom1;
    double Geom2;
    double Geom3;
    double Geom4;
    int Barrels;
    int Culvert;
};

struct I_Timeseries
{
    std::string Name;
    std::string Date;
    std::string Time;
    double Value;
};

struct I_Report
{
    std::string Type;
    std::string Options;
};

struct I_Coordinates
{
    std::string Node;
    std::string Xcoord;
    std::string Ycoord;
};

struct I_Polygons
{
    std::string Subcatchment;
    std::string Xcoord;
    std::string Ycoord;
};

struct I_Symbols
{
    std::string Gage;
    std::string Xcoord;
    std::string Ycoord;    
};

extern I_Options I_options;
extern I_Evaporation I_evaporation;
extern I_Raingage I_raingage;
extern I_Subarea I_subarea;
extern I_Infiltration I_infiltration;
extern I_Subcatchment I_subcatchment;
extern I_Junctions I_junctions;
extern I_Outfalls I_outfalls;
extern I_Conduit I_conduit;
extern I_Pumps I_pumps;
extern I_Xsections I_xsections;
extern I_Losses I_losses;
extern I_Timeseries I_timeseries;
extern I_Report I_report;
extern I_Coordinates I_coordinates;
extern I_Polygons I_polygons;
extern I_Symbols I_symbols;
extern std::vector <std::string> I_titleData;
extern std::vector <I_Options> I_optionData;
extern std::vector <I_Evaporation> I_evaporationData;
extern std::vector <I_Raingage> I_raingageData;
extern std::vector <I_Subcatchment> I_subcatchmentData;
extern std::vector <I_Subarea> I_subareaData;
extern std::vector <I_Infiltration> I_infiltrationData;
extern std::vector <I_Junctions> I_junctionData;
extern std::vector <I_Outfalls> I_outfallData;
extern std::vector <I_Conduit> I_conduitData;
extern std::vector <I_Pumps> I_pumpData;
extern std::vector <I_Xsections> I_xsectionData;
extern std::vector <I_Losses> I_lossesData;
extern std::vector <std::string> I_controlData;
extern std::vector <I_Timeseries> I_timeseriesData;
extern std::vector <I_Report> I_reportData;
extern std::vector <std::string> I_tagData;
extern std::vector <std::string> I_mapData;
extern std::vector <I_Coordinates> I_coordinateData;
extern std::vector <std::string> I_verticeData;
extern std::vector <I_Polygons> I_polygonData;
extern std::vector <I_Symbols> I_symbolData;

extern int SubcatchNum;
extern int LinkNum;
extern void read_WLS_File(const std::string &WLS_File);
extern void readSWMMINPFile(const std::string &inputFile);
extern void updateSWMMINP(const std::string &updatedSWMMINPFile);

#endif //IOSWMM_H