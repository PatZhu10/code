#include "ioSWMM.h"

// =========================================================
// WLS file
// =========================================================
WLS_NodeHead WLS_nodeHead;
std::vector <double> AllSimNodeHead;
std::vector <double> WLS_simNodeHead;
std::vector <std::string> WLS_stationData;
std::vector <WLS_NodeHead> WLS_nodeHeadData;

// =========================================================
// SWMM INP file
// =========================================================
I_Options I_options;
I_Evaporation I_evaporation;
I_Raingage I_raingage;
I_Subarea I_subarea;
I_Infiltration I_infiltration;
I_Subcatchment I_subcatchment;
I_Junctions I_junctions;
I_Outfalls I_outfalls;
I_Storage I_storage;
I_Conduit I_conduit;
I_Pumps I_pumps;
I_Xsections I_xsections;
I_Losses I_losses;
I_Timeseries I_timeseries;
I_Report I_report;
I_Coordinates I_coordinates;
I_Polygons I_polygons;
I_Symbols I_symbols;
std::vector <std::string> I_titleData;
std::vector <I_Options> I_optionData;
std::vector <I_Evaporation> I_evaporationData;
std::vector <I_Raingage> I_raingageData;
std::vector <I_Subcatchment> I_subcatchmentData;
std::vector <I_Subarea> I_subareaData;
std::vector <I_Infiltration> I_infiltrationData;
std::vector <I_Junctions> I_junctionData;
std::vector <I_Outfalls> I_outfallData;
std::vector <I_Storage> I_storageData;
std::vector <I_Conduit> I_conduitData;
std::vector <I_Pumps> I_pumpData;
std::vector <I_Xsections> I_xsectionData;
std::vector <I_Losses> I_lossesData;
std::vector <std::string> I_controlData;
std::vector <std::string> I_curveData;
std::vector <I_Timeseries> I_timeseriesData;
std::vector <I_Report> I_reportData;
std::vector <std::string> I_tagData;
std::vector <std::string> I_mapData;
std::vector <I_Coordinates> I_coordinateData;
std::vector <std::string> I_verticeData;
std::vector <I_Polygons> I_polygonData;
std::vector <I_Symbols> I_symbolData;

int SubcatchNum;
int LinkNum;

//=============================================================================
// Read WLS File
//=============================================================================
void read_WLS_File(const std::string &WLS_File)
{
    std::ifstream WLS_file(WLS_File);
    std::string line;
    std::string section;
    std::string dummy;

// Read the WLS file
    if (!WLS_file.is_open())
    {
        // Handle error opening the file               

        throw std::runtime_error("Failed to open file: " + WLS_File);
    }
    else
    {
        // Handle any read errors if necessary
        if (!WLS_file) {
            // Handle error reading the file
            throw std::runtime_error("Error reading file: " + WLS_File);
        }
        else
        {
            while (std::getline(WLS_file, line))
            {   
                // check if at start of a new input section
                if (line[0] == '[')
                {
                    // match line against list of section keywords
                    section = line.substr(1, line.find(']') - 1);
                }
                else
                {
                    if (section == "WATER_LEVEL_STATION_ID")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            WLS_stationData.push_back(line);
                        }
                    }
                    else if (section == "REAL_NODE_HEAD")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> WLS_nodeHead.name >> WLS_nodeHead.value;

                            WLS_nodeHeadData.push_back(WLS_nodeHead);
                        }
                    }
                }
            }

            if (!WLS_file.eof())
            {
                // Handle error reading the file
                throw std::runtime_error("Error reading file: " + WLS_File);
            }

        }
    }
}

//=============================================================================
// Read SWMM INP File
//=============================================================================
void readSWMMINPFile(const std::string &inputFile)
{
    std::ifstream inputfile(inputFile);
    std::string line;
    std::string section;

// Read the original SWMM INP file
    if (!inputfile.is_open())
    {
        // Handle error opening the file           

        throw std::runtime_error("Failed to open file: " + inputFile);
    }
    else
    {
        // Handle any read errors if necessary
        if (!inputfile)
        {
            // Handle error reading the file
            throw std::runtime_error("Error reading file: " + inputFile);
        }
        else
        {
            while (getline(inputfile, line))
            {
                // check if at start of a new input section
                if (line[0] == '[')
                {
                    // match line against list of section keywords
                    section = line.substr(1, line.find(']') - 1);
                }
                    if (section == "TITLE")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            I_titleData.push_back(line);
                        }
                    }

                    else if (section == "OPTIONS")
                    {
                        
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_options.Option >> I_options.Value;
                            I_optionData.push_back(I_options);
                        }                          
                    }
                    else if (section == "EVAPORATION")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_evaporation.Data_Source >> I_evaporation.Parameters;
                            I_evaporationData.push_back(I_evaporation);
                        }
                    }
                    else if (section == "RAINGAGES")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_raingage.Name >> I_raingage.Format >> I_raingage.Interval
                                >> I_raingage.SCF >> I_raingage.Source;

                            I_raingageData.push_back(I_raingage);
                        }
                    }
                    else if (section == "SUBCATCHMENTS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_subcatchment.Name >> I_subcatchment.Raingage >> I_subcatchment.Outlet
                                >> I_subcatchment.Area >> I_subcatchment.PctImperv >> I_subcatchment.Width
                                >> I_subcatchment.Slope;
                            
                            I_subcatchmentData.push_back(I_subcatchment);
                        }
                    }
                    else if (section == "SUBAREAS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_subarea.Name >> I_subarea.NImperv >> I_subarea.NPerv
                                >> I_subarea.SImperv >> I_subarea.SPerv >> I_subarea.PctZero
                                >> I_subarea.RouteTo >> I_subarea.PctRouted;                            
                            I_subareaData.push_back(I_subarea);
                        }
                    }
                    else if (section == "INFILTRATION")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_infiltration.Name >> I_infiltration.Param1 >> I_infiltration.Param2
                                >> I_infiltration.Param3 >> I_infiltration.Param4 >> I_infiltration.Param5;
                            I_infiltrationData.push_back(I_infiltration);
                        }
                    }
                    else if (section == "JUNCTIONS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_junctions.Name >> I_junctions.Elevation >> I_junctions.Maxdepth
                                >> I_junctions.InitDepth >> I_junctions.SurDepth >> I_junctions.Aponded;
                            I_junctionData.push_back(I_junctions);
                        }
                    }
                    else if (section == "OUTFALLS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_outfalls.Name >> I_outfalls.Elevation >> I_outfalls.Type
                                >> I_outfalls.StageData >> I_outfalls.Gated >> I_outfalls.RouteTo;
                            I_outfallData.push_back(I_outfalls);
                        }
                    }
                    else if (section == "STORAGE")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_storage.Name >> I_storage.Elevation >> I_storage.Ymax
                                >> I_storage.Y0 >> I_storage.Curve_type >> I_storage.A1 
                                >> I_storage.A2 >> I_storage.A0;
                            I_storageData.push_back(I_storage);
                        }
                    }                    
                    else if (section == "CONDUITS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_conduit.Name >> I_conduit.FromNode >> I_conduit.ToNode
                                >> I_conduit.Length >> I_conduit.Roughness >> I_conduit.InOffset
                                >> I_conduit.OutOffset >> I_conduit.InitFlow >> I_conduit.MaxFlow;
                            I_conduitData.push_back(I_conduit);
                        }
                    }
                    else if (section == "PUMPS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_pumps.Name >> I_pumps.FromNode >> I_pumps.ToNode
                                >> I_pumps.PumpCurve >> I_pumps.Status >> I_pumps.Startup
                                >> I_pumps.Shutoff;
                            I_pumpData.push_back(I_pumps);
                        }
                    }
                    else if (section == "XSECTIONS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_xsections.Link >> I_xsections.Shape >> I_xsections.Geom1
                                >> I_xsections.Geom2 >> I_xsections.Geom3 >> I_xsections.Geom4
                                >> I_xsections.Barrels >> I_xsections.Culvert;
                            I_xsectionData.push_back(I_xsections);
                        }
                    }
                    else if (section == "LOSSES")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {

                            std::istringstream iss(line);
                            iss >> I_losses.Conduit >> I_losses.Kentry >> I_losses.Kexit
                                >> I_losses.Kavg >> I_losses.Flap >> I_losses.Seepage;
                            I_lossesData.push_back(I_losses);
                        }
                    }
                    else if (section == "CONTROLS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            I_controlData.push_back(line);
                        }
                    }
                    else if (section == "CURVES")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            I_curveData.push_back(line);
                        }
                    }
                    else if (section == "TIMESERIES")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_timeseries.Name >> I_timeseries.Date >> I_timeseries.Time
                                >> I_timeseries.Value;
                            I_timeseriesData.push_back(I_timeseries);
                        }
                    }
                    else if (section == "REPORT")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_report.Type >> I_report.Options;
                            I_reportData.push_back(I_report);
                        }
                    }
                    else if (section == "TAGS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            I_tagData.push_back(line);
                        }
                    }
                    else if (section == "MAP")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            I_mapData.push_back(line);

                        }
                    }
                    else if (section == "COORDINATES")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_coordinates.Node >> I_coordinates.Xcoord >> I_coordinates.Ycoord;
                            I_coordinateData.push_back(I_coordinates);
                        }
                    }
                    else if (section == "VERTICES")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            I_verticeData.push_back(line);
                        }
                    }
                    else if (section == "Polygons")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_polygons.Subcatchment >> I_polygons.Xcoord >> I_polygons.Ycoord;
                            I_polygonData.push_back(I_polygons);
                        }
                    }
                    else if (section == "SYMBOLS")
                    {
                        if (line[0] == ';' || line[0] == '[' || line.length() < 2)
                        {
                            continue;
                        }
                        else
                        {
                            std::istringstream iss(line);
                            iss >> I_symbols.Gage >> I_symbols.Xcoord >> I_symbols.Ycoord;
                            I_symbolData.push_back(I_symbols);
                        }
                    }
            }
            SubcatchNum = I_subcatchmentData.size();
            LinkNum = I_conduitData.size();
        }
        if (!inputfile.eof())
        {
            // Handle error reading the file
            throw std::runtime_error("Error reading file: " + inputFile);
        }
    }
}

//=============================================================================
// Update SWMM INP File
//=============================================================================
void updateSWMMINP(const std::string &updatedSWMMINPFile)
{
    std::ostringstream oss;
    std::ofstream outputfile(updatedSWMMINPFile);

// Write [TITLE]
    oss << "[TITLE]" << "\n";
    oss << ";;Project Title/Notes" << "\n";
    for (const auto& title : I_titleData)
    {
        oss << title << "\n";
    }
    oss << "\n";

// Write [OPTIONS]
    oss << "[OPTIONS]" << "\n";
    oss << ";;Option             Value" << "\n";
    for (const auto& option : I_optionData)
    {
        oss << option.Option << "\t" << option.Value << "\n";
    }
    oss << "\n";

// Write [EVAPORATION]
    oss << "[EVAPORATION]" << "\n";
    oss << ";;Data Source    Parameters" << "\n";
    oss << ";;-------------- ----------------" << "\n";
    for (const auto& evaporation : I_evaporationData)
    {
        oss << evaporation.Data_Source << "\t" << evaporation.Parameters << "\n";
    }
    oss << "\n";

// Write [RAINGAGES]
    oss << "[RAINGAGES]" << "\n";
    oss << ";;Name           Format    Interval SCF      Source" << "\n";
    oss << ";;-------------- --------- ------ ------ ----------" << "\n";
    for (const auto& raingage : I_raingageData)
    {
        oss << raingage.Name << "\t" << raingage.Format << "\t" << raingage.Interval << "\t"
            << raingage.SCF << "\t" << raingage.Source << "\t" << I_timeseries.Name << "\n";
    }
    oss << "\n";

// Write [SUBCATCHMENTS]
    oss << "[SUBCATCHMENTS]" << "\n";
    oss << ";;Name           Rain Gage        Outlet           Area     '%'Imperv  Width    '%'Slope   CurbLen" << "\n";
    oss << ";;-------------- ---------------- ---------------- -------- -------- -------- -------- --------" << "\n";
    for (std::size_t i = 0; i < I_subcatchmentData.size(); i++)
    {
        const I_Subcatchment& subcatchment = I_subcatchmentData[i];
        oss << subcatchment.Name << "\t" << subcatchment.Raingage << "\t" << subcatchment.Outlet << "\t"
            << subcatchment.Area << "\t" << best_swmmParam.stored_subcatchFracImperv[i] << "\t" << subcatchment.Width << "\t"
            << subcatchment.Slope << "\t" << subcatchment.CurbLen << "\n";
    }
    oss << "\n";

// Write [SUBAREAS]
    oss << "[SUBAREAS]" << "\n";
    oss << ";;Subcatchment   N-Imperv   N-Perv     S-Imperv   S-Perv     PctZero    RouteTo    PctRouted " << "\n";
    oss << ";;-------------- ---------- ---------- ---------- ---------- ---------- ---------- ----------" << "\n";
    for (std::size_t i = 0; i < I_subareaData.size(); i++)
    {
        const I_Subarea& subarea = I_subareaData[i];
        
        oss << subarea.Name << "\t" << best_swmmParam.stored_subcatchImperv01N[i] << "\t" << best_swmmParam.stored_subcatchPerv01N[i] << "\t"
            << best_swmmParam.stored_subcatchImperv1DStore[i] << "\t" << best_swmmParam.stored_subcatchPerv1DStore[i] << "\t" << subarea.PctZero << "\t"
            << subarea.RouteTo << "\t" << subarea.PctRouted << "\n";
    }
    oss << "\n";

// Write [INFILTRATION]
    oss << "[INFILTRATION]" << "\n";
    oss << ";;Subcatchment   Param1     Param2     Param3     Param4     Param5" << "\n";
    oss << ";;-------------- ---------- ---------- ---------- ---------- ----------" << "\n";
    for (std::size_t i = 0; i < I_subcatchmentData.size(); i++)
    {
        const I_Subcatchment& subcatchment = I_subcatchmentData[i];
        const I_Infiltration& infiltration = I_infiltrationData[i];
        oss << subcatchment.Name << "\t" << best_swmmParam.stored_subcatchF0[i] << '\t' << best_swmmParam.stored_subcatchFMin[i] << "\t"
            << best_swmmParam.stored_subcatchDecay[i] << "\t" << infiltration.Param4 << "\t" << infiltration.Param5 << "\n";
    }
    oss << "\n";

// Write [JUNCTIONS]
    oss << "[JUNCTIONS]" << "\n";
    oss << ";;Name           Elevation  MaxDepth   InitDepth  SurDepth   Aponded" << "\n";
    oss << ";;-------------- ---------- ---------- ---------- ---------- ----------" << "\n";
    for (const auto& junction: I_junctionData)
    {
        oss << junction.Name << "\t" << junction.Elevation << "\t" << junction.Maxdepth << "\t"
            << junction.InitDepth << "\t" << junction.SurDepth << "\t" << junction.Aponded << "\n";
    }
    oss << "\n";

// Write [OUTFALLS]
    oss << "[OUTFALLS]" << "\n";
    oss << ";;Name           Elevation  Type       Stage Data       Gated    Route To" << "\n";
    oss << ";;-------------- ---------- ---------- ---------------- -------- ----------------" << "\n";
    for (const auto& outfalls : I_outfallData)
    {
        oss << outfalls.Name << "\t" << outfalls.Elevation << "\t" << outfalls.Type << "\t"
            << outfalls.StageData << "\t" << outfalls.Gated << "\t" << outfalls.RouteTo << "\n";
    }
    oss << "\n";

// Write [STORAGE]
   oss << "[STORAGE]" << "\n";
   oss << ";;Name           Elev.    MaxDepth   InitDepth  Curve Type  A1 A2 A0" << "\n";
   oss << ";;-------------- -------- ---------- ----------- ---------- -- -- --" << "\n";
   for (const auto& storage : I_storageData)
   {
        oss << storage.Name << "\t" << storage.Elevation << "\t" << storage.Ymax << "\t" 
            << storage.Y0 << "\t" << storage.Curve_type << "\t" << storage.A1 << "\t" 
            << storage.A2 << "\t" << storage.A0 << "\n";
   }
   oss << "\n";

// Write [CONDUITS]
    oss << "[CONDUITS]" << "\n";
    oss << ";;Name           From Node        To Node          Length     Roughness  InOffset   OutOffset  InitFlow   MaxFlow" << "\n";
    oss << ";;-------------- ---------------- ---------------- ---------- ---------- ---------- ---------- ---------- ----------" << "\n";
    for (std::size_t i = 0; i < I_conduitData.size(); i++)
    {
        const I_Conduit& conduit = I_conduitData[i];
        oss << conduit.Name << "\t" << conduit.FromNode << "\t" << conduit.ToNode << "\t"
            << conduit.Length << "\t" << best_swmmParam.stored_LinkManning[i] << "\t" << conduit.InOffset << "\t"
            << conduit.OutOffset << "\t" << conduit.InitFlow << "\t" << conduit.MaxFlow << "\n";
    }
    oss << "\n";

// Write [PUMPS]
    oss << "[PUMPS]" << "\n";
    oss << ";;Name           From Node        To Node          Pump Curve       Status   Sartup Shutoff" << "\n";
    oss << ";;-------------- ---------------- ---------------- ---------------- ------ -------- --------" << "\n";
    for (const auto& pumps : I_pumpData)
    {
        oss << pumps.Name << "\t" << pumps.FromNode << "\t" << pumps.ToNode << "\t"
            << pumps.PumpCurve << "\t" << pumps.Status << "\t" << pumps.Startup << "\t"
            << pumps.Shutoff << "\n";
    }
    oss << "\n";

// Write [XSECTIONS]
    oss << "[XSECTIONS]" << "\n";
    oss << ";;Link           Shape        Geom1            Geom2      Geom3      Geom4      Barrels    Culvert" << "\n";
    oss << ";;-------------- ------------ ---------------- ---------- ---------- ---------- ---------- ----------" << "\n";
    for (const auto& xsections : I_xsectionData)
    {
        oss << xsections.Link << "\t" << xsections.Shape << "\t" << xsections.Geom1 << "\t"
            << xsections.Geom2 << "\t" << xsections.Geom3 << "\t" << xsections.Geom4 << "\t"
            << xsections.Barrels << "\t" << xsections.Culvert << "\n";
    }
    oss << "\n";

// Write [LOSSES]
    oss << "[LOSSES]" << "\n";
    oss << ";Conduit    Kentry      Kexit       Kavg        Flap        Seepage" << "\n";
    oss << ";-------    ------      -------     -------     -------     ---------" << "\n";
    for (std::size_t i = 0; i < I_lossesData.size(); i++)
    {
        const I_Losses& losses = I_lossesData[i];
        oss << losses.Conduit << "\t" << losses.Kentry << "\t" << losses.Kexit << "\t"
            << losses.Kavg << "\t" << losses.Flap << "\t"<< losses.Seepage << "\n";
    }
    oss << "\n";

// Write [CONTROLS]
    oss << "[CONTROLS]" << "\n";
    oss << ";--------------------------------------------" << "\n";
    for (std::size_t i = 0; i < I_controlData.size(); i++)
    {
        oss << I_controlData[i] << "\n";
    }
    oss << "\n";

// Write [CURVES]
    oss << "[CURVES]" << "\n";
    oss << ";;Name           Type       X-Value    Y-Value" << "\n";
    oss << ";;-------------- ---------- ---------- ----------" << "\n";
    for (std::size_t i = 0; i < I_curveData.size(); i++)
    {
        oss << I_curveData[i] << "\n";
    }
    oss << "\n";

// Write [TIMESERIES]
    oss << "[TIMESERIES]" << "\n";
    oss << ";;Name           Date       Time       Value" << "\n";
    oss << ";;-------------- ---------- ---------- ----------" << "\n";
    for (const auto& timeseries : I_timeseriesData)
    {
        oss << timeseries.Name << "\t" << timeseries.Date << "\t" << timeseries.Time << "\t"
            << timeseries.Value << "\n";
    }
    oss << "\n";

// Write [REPORT]
    oss << "[REPORT]" << "\n";
    oss << ";;Reporting Options" << "\n";
    for (const auto& report : I_reportData)
    {
        oss << report.Type << "\t" << report.Options << "\n";
    }
    oss << "\n";

// Write [TAGS]
    oss << "[TAGS]" << "\n";
    for (std::size_t i = 0; i < I_tagData.size(); i++)
    {
        oss << I_tagData[i] << "\n";
    }
    oss << "\n";

// Write [MAP]
    oss << "[MAP]" << "\n";
    for (std::size_t i = 0; i < I_mapData.size(); i++)
    {
        oss << I_mapData[i] << "\n";
    }
    oss << "\n";

// Write [COORDINATES]
    oss << "[COORDINATES]" << "\n";
    oss << ";;Node           X-Coord            Y-Coord" << "\n";
    oss << ";;-------------- ------------------ ------------------" << "\n";
    for (const auto& coordinates : I_coordinateData)
    {
        oss << coordinates.Node << "\t" << coordinates.Xcoord << "\t" << coordinates.Ycoord << "\n";
    }
    oss << "\n";

// Write [VERTICES]
    oss << "[VERTICES]" << "\n";
    oss << ";;Link           X-Coord            Y-Coord" << "\n";
    oss << ";;-------------- ------------------ ------------------" << "\n";
    for (std::size_t i = 0; i < I_verticeData.size(); i++)
    {
        oss << I_verticeData[i] << "\n";
    }
    oss << "\n";

// Write [Polygons]
    oss << "[Polygons]" << "\n";
    oss << ";;Subcatchment   X-Coord            Y-Coord" << "\n";
    oss << ";;-------------- ------------------ ------------------" << "\n";
    for (const auto& polygons : I_polygonData)
    {
        oss << polygons.Subcatchment << "\t" << polygons.Xcoord << "\t" << polygons.Ycoord << "\n";
    }
    oss << "\n";    

// Write [SYMBOLS]
    oss << "[SYMBOLS]" << "\n";
    oss << ";;Gage           X-Coord            Y-Coord" << "\n";
    oss << ";;-------------- ------------------ ------------------" << "\n";
    for (const auto& symbols : I_symbolData)
    {
        oss << symbols.Gage << "\t" << symbols.Xcoord << "\t" << symbols.Ycoord << "\n";
    }
    oss << "\n";    

    std::string result = oss.str();

    if (outputfile.is_open())
    {
        outputfile << result;
        outputfile.close();
    }
    else
    {
        std::cerr << "Error opening the file" << "\n";
    }
}

//=============================================================================
// Output All Node Head to Text File
//=============================================================================
void outputNodeHeadToFile(const std::string &outputNodeHeadFile, const std::vector<double> &WLS_AllSimNodeHead)
{
    std::ostringstream oss;

    // Populate the ostringstream with data from the vector
    for (const auto& value : WLS_AllSimNodeHead)
    {
        oss << value << "\n";
    }

    // Convert the ostringstream to a string
    std::string result = oss.str();

    // Open the output file
    std::ofstream outputfile(outputNodeHeadFile);

    // Check if the file is open
    if (outputfile.is_open())
    {
        // Write the string to the file
        outputfile << result;
        outputfile.close();
    }
    else
    {
        std::cerr << "Error opening the file" << "\n";
    }
}