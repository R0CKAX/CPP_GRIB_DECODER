#include "Decode.hpp"

Decode::Decode()
{
	check = NULL;

	fileName = "default.grib", codingName = "", s_flag = "", s_originCentre = "", s_pid = "", s_hour = "", s_minute = "", calculated_latitude = 0, calculated_longtitude = 0;
	s_resAndCompFlags = "", calculatedLatitudeLastPoint = 0, calculatedLongtitudeLastPoint = 0, calculatedLongtitudalDirIncrement = 0, calculatedLatitudalDirIncrement = 0, 
	s_scanningModeFlags = "";

	section0_start = 0, fileEnd = 0, messageLength = 0, editionNumber = 0;

	section1_start = 0, section1_length = 0, versionNumber = 0,originCentre = 0, pid = 0, gridDef = 0, flag = 0, parameterIndicator = 0, lvlTypeIndicator = 0, height_pressure = 0, 
	year = 0, month = 0, day = 0, hour = 0, minute = 0, unitTimeRange = 0, periodOfTime = 0, intervalTime = 0, timeRangeIndicator = 0, numberIncluded = 0, numberMissing = 0, 
	centuryReference = 0, subCentreIdentification = 0, decimalScaleFactor = 0;

	section2_start = 0, section2_length = 0, numberOfVerticals = 0, location = 0, dataRepresentationType = 0, numOfPointsLatitude = 0, numOfPointsLongitude = 0, latitude_1 = 0,
	latitude_2 = 0,	longtitude_1 = 0, longtitude_2 = 0, resAndCompFlags = 0, latitudeLastPoint = 0, longtitudeLastPoint = 0, longtitudalDirIncrement = 0;
	latitudalDirIncrement = 0, scanningModeFlags = 0;

	section3_start = 0, section3_length = 0, unusedBits = 0, tableReference = 0;

	section4_start = 0, section4_length = 0, flag_section4 = 0, scaleFactor = 0;
}

uint32_t Decode::readThreeBytes(uint32_t mPos) const
{
	uint32_t value = 0;
	char tmp, tmp2, tmp3;

	std::fstream file;
	file.open(fileName, std::ios::binary | std::ios::in);

	file.seekg(mPos);
	file.read(&tmp, 1);
	file.read(&tmp2, 1);
	file.read(&tmp3, 1);

	value = (uint8_t)tmp << 16 | (uint8_t)tmp2 << 8 | (uint8_t)tmp3 & 0xFF;
	file.close();

	return value;
}

uint16_t Decode::readTwoBytes(uint32_t mPos) const
{
	uint16_t value = 0;
	char tmp, tmp2;

	std::fstream file;
	file.open(fileName, std::ios::binary | std::ios::in);

	file.seekg(mPos);
	file.read(&tmp, 1);
	file.read(&tmp2, 1);

	value = (uint8_t)tmp << 8 | (uint8_t)tmp2 & 0xFF;
	file.close();

	return value;
}

uint8_t Decode::readOneByte(uint32_t mPos) const
{
	uint8_t value = 0;
	char tmp;

	std::fstream file;
	file.open(fileName, std::ios::binary | std::ios::in);

	file.seekg(mPos);
	file.read(&tmp, 1);

	value = (uint8_t)tmp & 0xFF;
	file.close();

	return value;
}

void Decode::loadSection0()
{
	char byte, byte2, byte3, byte4;

	std::fstream file;
	file.open(fileName, std::ios::binary | std::ios::in);

	file.seekg(19);
	for (int i = 0; i < 6; i++)
	{
		uint8_t tmp = readOneByte(19 + i);
		codingName += tmp;
	}

	while (!file.eof())
	{
		file.read(&byte, 1);
		if (byte == 'G')
		{
			file.read(&byte2, 1);
			file.read(&byte3, 1);
			file.read(&byte4, 1);
			if (byte2 == 'R' && byte3 == 'I' && byte4 == 'B')
			{
				section0_start = static_cast<uint32_t>(file.tellg()) - 4;
				break;
			}
		}
	}

	while (!file.eof())
	{
		file.read(&byte, 1);
		if (byte == '7')
		{
			file.read(&byte2, 1);
			file.read(&byte3, 1);
			file.read(&byte4, 1);
			if (byte2 == '7' && byte3 == '7' && byte4 == '7')
			{
				fileEnd = static_cast<uint32_t>(file.tellg()) - 4;
				break;
			}
		}
	}

	messageLength = readThreeBytes(section0_start + 4);
	editionNumber = readOneByte(section0_start + 7);
}

void Decode::loadSection1()
{
	section1_start = (section0_start + 8);
	section1_length = readThreeBytes(section1_start);
	versionNumber = readOneByte(section1_start + 3);
	originCentre = readOneByte(section1_start + 4);
	pid = readOneByte(section1_start + 5);
	gridDef = readOneByte(section1_start + 6);
	flag = readOneByte(section1_start + 7);
	parameterIndicator = readOneByte(section1_start + 8);
	lvlTypeIndicator = readOneByte(section1_start + 9);
	height_pressure = readTwoBytes(section1_start + 10);
	year = readOneByte(section1_start + 12);
	month = readOneByte(section1_start + 13);
	day = readOneByte(section1_start + 14);
	hour = readOneByte(section1_start + 15);
	minute = readOneByte(section1_start + 16);
	unitTimeRange = readOneByte(section1_start + 17);
	periodOfTime = readOneByte(section1_start + 18);
	intervalTime = readOneByte(section1_start + 19);
	timeRangeIndicator = readOneByte(section1_start + 20);
	numberIncluded = readTwoBytes(section1_start + 21);
	numberMissing = readOneByte(section1_start + 23);
	centuryReference = readOneByte(section1_start + 24);
	subCentreIdentification = readOneByte(section1_start + 25);
	decimalScaleFactor = readTwoBytes(section1_start + 26);
}

void Decode::loadSection2()
{
	section2_start = section1_start + section1_length;
	section2_length = readThreeBytes(section2_start);
	numberOfVerticals = readOneByte(section2_start + 3);
	location = readOneByte(section2_start + 4);
	dataRepresentationType = readOneByte(section2_start + 5);
	numOfPointsLatitude = readTwoBytes(section2_start + 6);
	numOfPointsLongitude = readTwoBytes(section2_start + 8);
	latitude_1 = readOneByte(section2_start + 10);
	latitude_2 = readTwoBytes(section2_start + 11);
	longtitude_1 = readOneByte(section2_start + 13);
	longtitude_2 = readTwoBytes(section2_start + 14);
	resAndCompFlags = readOneByte(section2_start + 16);
	latitudeLastPoint = readThreeBytes(section2_start + 17);
	longtitudeLastPoint = readThreeBytes(section2_start + 20);
	longtitudalDirIncrement = readTwoBytes(section2_start + 23);
	latitudalDirIncrement = readTwoBytes(section2_start + 25);
	scanningModeFlags = readOneByte(section2_start + 28);

	for (uint32_t i = 0; i < (section2_length - 32) / 2; i++)
	{
		uint16_t tmp = readTwoBytes(section2_start + 32 + 2 * i);
		listOfNumbers.push_back(tmp);
	}
}

void Decode::loadSection3()
{
	section3_length = readThreeBytes(section3_start);
	unusedBits = readOneByte(section3_start + 3);
	tableReference = readTwoBytes(section3_start + 4);
}

void Decode::loadSection4()
{
	section4_length = readThreeBytes(section4_start);
	flag_section4 = readOneByte(section4_start + 3);
	scaleFactor = readTwoBytes(section4_start + 4);
}

void Decode::loadFile()
{
	system("cls");
	std::cout << "Podaj jaki plik .grib chcia³(a)byœ wczytaæ: ";
	std::cin >> fileName;
	fileName += ".grib";

	std::fstream file;
	file.open(fileName, std::ios::binary | std::ios::in);

	if (file.good())
	{
		check = '1';
		loadSection0();
		loadSection1();

		if (flag == 0)
		{
			section4_start = section1_start + section1_length;
		}
		if (flag == 64)
		{
			section3_start = section1_start + section1_length;
			loadSection3();

			section4_start = section3_start + section3_length;
		}
		else if (flag == 128)
		{
			loadSection2();

			section4_start = section2_start + section2_length;
		}
		else if (flag == 192)
		{
			loadSection2();
			section3_start = section2_start + section2_length;
			loadSection3();

			section4_start = section3_start + section3_length;
		}
		
		loadSection4();

		std::cout << "Pomyœlnie za³adowano dane z pliku " << fileName << "\n";
		setProperValues();

		_getch();
	}
	else
	{
		std::cout << "Nie uda³o siê otworzyæ pliku\n";
	}

	file.close();
}

void Decode::setProperValues()
{
	if (flag == 64)
	{
		s_flag = "GDS Omitted, BMS Included";
	}
	else if (flag == 128)	
	{
		s_flag = "GDS Included, BMS Omitted";
	}
	else if (flag == 192)
	{
		s_flag = "GDS Included, BMS Included";
	}
	else if (flag == 0)
	{
		s_flag = "GDS Omitted, BMS Omitted";
	}

	if (hour < 10)
	{
		s_hour = "0" + std::to_string(hour);
	}
	else
	{
		s_hour = std::to_string(hour);
	}

	if (minute < 10)
	{
		s_minute = "0" + std::to_string(minute);
	}
	else
	{
		s_minute = std::to_string(minute);
	}

	std::map<int, std::string> originCentreName = {
		{0, "WMO Secretariat"},{1, "Melbourne"},{2, "Melbourne"},{3, "Melbourne"},{4, "Moscow"},{5, "Moscow"},{6, "Moscow"},{7, "US National Weather Service - NCEP (WMC)"},
		{8, "US National Weather Service Telecommunications Gateway (NWSTG)"},{9, "US National Weather Service - Other"},{10, "Cairo (RSMC)"},{11, "Cairo (RSMC)"},
		{12, "Dakar (RSMC)"},{13, "Dakar (RSMC)"},{14, "Nairobi (RSMC)"},{15, "Nairobi (RSMC)"},{16, "Casablanca (RSMC)"},{17, "Tunis (RSMC)"},{18, "Tunis Casablanca (RSMC)"},
		{19, "Tunis Casablanca (RSMC)"},{20, "Las Palmas"},{21, "Algiers (RSMC)"},{22, "ACMAD"},{23, "Mozambique NMC"},{24, "Pretoria (RSMC)"},{25, "La Reunion (RSMC)"},
		{26, "Khabarovsk (RSMC)"},{27, "Khabarovsk (RSMC)"},{28, "New Delhi (RSMC)"},{29, "New Delhi (RSMC)"},{30, "Novosibirsk (RSMC)"},{31, "Novosibirsk (RSMC)"},
		{32, "Tashkent (RSMC)"},{33, "Jeddah (RSMC)"},{34, "Tokyo (RSMC), Japan Meteorological Agency"},{35, "Tokyo (RSMC), Japan Meteorological Agency"},{36, "Bangkok"},
		{37, "Ulan Bator"},{38, "Beijing (RSMC)"},{39, "Beijing (RSMC)"},{40, "Seoul"},{41, "Buenos Aires (RSMC)"},{42, "Buenos Aires (RSMC)"},{43, "Brasilia (RSMC)"},
		{44, "Brasilia (RSMC)"},{45, "Santiago"},{46, "Brazilian Space Agency INPE"},{47, "Colombia NMC"},{48, "Ecuador NMC"},{49, "Peru NMC"},{50, "Venezuela NMC"},
		{51, "Miami (RSMC)"},{52, "Miami RSMC, National Hurricane Center"},{53, "Montreal (RSMC)"},{54, "Montreal (RSMC)"},{55, "San Francisco"},{56, "ARINC Centre"},
		{57, "U.S. Air Force Air Force Global Weather Central"},{58, "Fleet Numerical Meteorology and Oceanography Center, Monterey, CA"},
		{59, "The NOAA Forecast Systems Laboratory, Boulder, CO, USA"},{60, "United States National Centre for Atmospheric Research (NCAR"},{61, "Service ARGOS - Landover"},
		{62, "U.S. Naval Oceanographic Office"},{63, "IRI (International Research Institute for Climate and Society)"},{64, "Honolulu (RSMC)"},{65, "Darwin (RSMC)"},
		{66, "Darwin (RSMC)"},{67, "Melbourne (RSMC)"},{68, "RESERVED VALUE"},{69, "Wellington (RSMC)"},{70, "Wellington (RSMC)"},{71, "Nadi (RSMC)"},
		{72, "Singapore"},{73, "Malaysia NMC"},{74, "UK Meteorological Office Exeter (RSMC)"},{75, "UK Meteorological Office Exeter (RSMC)"},{76, "Moscow (RSMC)"},
		{77, "RESERVED VALUE"},{78, "Offenbach (RSMC)"},{79, "Offenbach (RSMC)"},{80, "Rome (RSMC)"},{81, "Rome (RSMC)"},{82, "Norrkoping"},{83, "Norrkoping"},
		{84, "Toulouse (RSMC)"},{85, "Toulouse (RSMC)"},{86, "Helsinki"},{87, "Belgrade"},{88, "Oslo"},{89, "Prague"},{90, "Episkopi"},{91, "Ankara"},{92, "Frankfurt/Main"},
		{93, "London (WAFC)"},{94, "Copenhagen"},{95, "Rota"},{96, "Athens"},{97, "European Space Agency (ESA)"},{98, "European Centre for Medium-range Weather Forecast (ECMWF), (RSMC)"},
		{99, "De Bilt"},{100, "Brazzaville"},{255, "Missing value"}
	};

	s_originCentre = originCentreName[originCentre];

	std::map<int, std::string> processIdNumbers = {
		{2, "Ultra Violet Index Model"},{3, "NCEP/ARL Transport and Dispersion Model"},{4, "NCEP/ARL Smoke Model"},
		{5, "Satellite Derived Precipitation and temperatures, from IR"},{6, "NCEP/ARL Dust Model"},{10, "Global Wind-Wave Forecast Model"},
		{11, "Global Multi-Grid Wave Model (Static Grids)"},{12, "Probabilistic Storm Surge (P-Surge)"},{13, "Hurricane Multi-Grid Wave Model"},{14, "Extra-tropical Storm Surge Atlantic Domain"},
		{15, "Nearshore Wave Prediction System (NWPS)"},{16, "Extra-Tropical Storm Surge (ETSS)"},{17, "Extra-tropical Storm Surge Pacific Domain"},
		{18, "Probabilistic Extra-Tropical Storm Surge (P-ETSS)"},{20, "Extra-tropical Storm Surge Micronesia Domain"},{21, "Extra-tropical Storm Surge Atlantic Domain (3D)"},
		{22, "Extra-tropical Storm Surge Pacific Domain (3D)"},{23, "Extra-tropical Storm Surge Micronesia Domain (3D)"},{25, "Snow Cover Analysis"},{30, "Forecaster generated field"},
		{31, "Value added post processed field"},{42, "Global Optimum Interpolation Analysis (GOI) from GFS model"},{43, "Global Optimum Interpolation Analysis (GOI) from 'Final' run"},
		{44, "Sea Surface Temperature Analysis"},{45, "Coastal Ocean Circulation Model"},{46, "HYCOM - Global"},{47, "HYCOM - North Pacific basin"},{48, "HYCOM - North Atlantic basin"},
		{49, "Ozone Analysis from TIROS Observations"},{52, "Ozone Analysis from Nimbus 7 Observations"},{64, "Regional Optimum Interpolation Analysis (ROI)"},
		{68, "80 wave triangular, 18-layer Spectral model from GFS model"},{69, "80 wave triangular, 18 layer Spectral model from 'Medium Range Forecast' run"},
		{70, "Quasi-Lagrangian Hurricane Model (QLM)"},{71, "Hurricane Weather Research and Forecasting (HWRF)"},{72, "Hurricane Non-Hydrostatic Multiscale Model on the B Grid (HNMMB)"},
		{73, "Fog Forecast model - Ocean Prod. Center"},{74, "Gulf of Mexico Wind/Wave"},{75, "Gulf of Alaska Wind/Wave"},{76, "Bias corrected Medium Range Forecast"},
		{77, "126 wave triangular, 28 layer Spectral model from GFS model"},{78, "126 wave triangular, 28 layer Spectral model from 'Medium Range Forecast' run"},
		{80, "62 wave triangular, 28 layer Spectral model from 'Medium Range Forecast' run"},{81, "Analysis from GFS (Global Forecast System)"},
		{82, "Analysis from GDAS (Global Data Assimilation System)"},{83, "High Resolution Rapid Refresh (HRRR)"},{84, "MESO NAM Model (currently 12 km)"},{85, "Real Time Ocean Forecast System (RTOFS)"},
		{86, "Early Hurricane Wind Speed Probability Model"},{87, "CAC Ensemble Forecasts from Spectral (ENSMB)"},{88, "NOAA Wave Watch III (NWW3) Ocean Wave Model"},
		{89, "Non-hydrostatic Meso Model (NMM) (Currently 8 km)"},{90, "62 wave triangular, 28 layer spectral model extension of the 'Medium Range Forecast' run"},
		{91, "62 wave triangular, 28 layer spectral model extension of the GFS model"},{92, "62 wave triangular, 28 layer spectral model run from the 'Medium Range Forecast' final analysis"},
		{93, "62 wave triangular, 28 layer spectral model run from the T62 GDAS analysis of the 'Medium Range Forecast' run"},{94, "T170/L42 Global Spectral Model from MRF run"},
		{95, "T126/L42 Global Spectral Model from MRF run"},{96, "Global Forecast System Model T1534 - Forecast hours 00-384, T574 - Forecast hours 00-192, T190 - Forecast hours 204-384"},
		{98, "Climate Forecast System Model -- Atmospheric model (GFS) coupled to a multi level ocean model. Currently GFS spectral model at T62, 64 levels coupled to 40 level MOM3 ocean model."},
		{99, "Miscellaneous Test ID"},{101, "Conventional Observation Re-Analysis (CORE)"},{104, "National Blend GRIB"},{105, "Rapid Refresh (RAP)"},
		{107, "Global Ensemble Forecast System (GEFS)"},{108, "Localized Aviation MOS Program (LAMP)"},{109, "Real Time Mesoscale Analysis (RTMA)"},{110, "NAM Model - 15km version"},
		{111, "NAM model, generic resolution (Used in SREF processing)"},{112, "WRF-NMM model, generic resolution (Used in various runs) NMM=Nondydrostatic Mesoscale Model (NCEP)"},
		{113, "Products from NCEP SREF processing"},{114, "NAEFS Products from joined NCEP, CMC global ensembles"},{115, "Downscaled GFS from NAM eXtension"},
		{116, "WRF-EM model, generic resolution (Used in various runs) EM - Eulerian Mass-core (NCAR - aka Advanced Research WRF)"},{117, "NEMS GFS Aerosol Component"},
		{118, "UnRestricted Mesoscale Analysis (URMA)"},{119, "WAM (Whole Atmosphere Model)"},{120, "Ice Concentration Analysis"},{121, "Western North Atlantic Regional Wave Model"},
		{122, "Alaska Waters Regional Wave Model"},{123, "North Atlantic Hurricane Wave Model"},{124, "Eastern North Pacific Regional Wave Model"},{125, "North Pacific Hurricane Wave Model"},
		{126, "Sea Ice Forecast Model"},{127, "Lake Ice Forecast Model"},{128, "Global Ocean Forecast Model"},{129, "Global Ocean Data Analysis System (GODAS)"},
		{130, "Merge of fields from the RUC, NAM, and Spectral Model"},{131, "Great Lakes Wave Model"},{132, "High Resolution Ensemble Forecast (HREF)"},{133, "Great Lakes Short Range Wave Model"},
		{134, "Rapid Refresh Forecast System (RRFS)"},{135, "Hurricane Analysis and Forecast System (HAFS)"},{140, "North American Regional Reanalysis (NARR)"},
		{141, "Land Data Assimilation and Forecast System"},{150, "NWS River Forecast System (NWSRFS)"},{151, "NWS Flash Flood Guidance System (NWSFFGS)"},
		{152, "WSR-88D Stage II Precipitation Analysis"},{153, "WSR-88D Stage III Precipitation Analysis"},{180, "Quantitative Precipitation Forecast generated by NCEP"},
		{181, "River Forecast Center Quantitative Precipitation Forecast mosaic generated by NCEP"},{182, "River Forecast Center Quantitative Precipitation estimate mosaic generated by NCEP"},
		{183, "NDFD product generated by NCEP/HPC"},{184, "Climatological Calibrated Precipitation Analysis (CCPA)"},{190, "National Convective Weather Diagnostic generated by NCEP/AWC"},
		{191, "Current Icing Potential automated product generated by NCEP/AWC"},{192, "Analysis product from NCEP/AWC"},{193, "Forecast product from NCEP/AWC"},{195, "Climate Data Assimilation System 2 (CDAS2)"},
		{196, "Climate Data Assimilation System 2 (CDAS2) - used for regeneration runs"},{197, "Climate Data Assimilation System (CDAS)"},{198, "Climate Data Assimilation System (CDAS) - used for regeneration runs"},
		{199, "Climate Forecast System Reanalysis (CFSR) -- Atmospheric model (GFS) coupled to a multi level ocean, land and seaice model. GFS spectral model at T382, 64 levels coupled to 40 level MOM4 ocean model."},
		{201, "CPC Automated Product"},{202, "EPA Air Quality Forecast - Currently North East US domain"},
        {203, "EPA Air Quality Forecast - Currently Eastern US domain"},{204, "SPC Manual Forecast Product"},{210, "NCEP/OPC automated product"},{211, "Reserved for WPC products"},
        {215, "Miscellaneous Test ID"},{216, "Miscellaneous Test ID"},{217, "Conventional Observation Re-Analysis (CORE)"},{218, "National Blend GRIB"},{219, "Rapid Refresh (RAP)"},
        {221, "Global Ensemble Forecast System (GEFS)"},{222, "Localized Aviation MOS Program (LAMP)"},{223, "Real Time Mesoscale Analysis (RTMA)"},{224, "NAM Model - 15km version"},
        {225, "NAM model, generic resolution (Used in SREF processing)"},{226, "WRF-NMM model, generic resolution (Used in various runs) NMM=Nondydrostatic Mesoscale Model (NCEP)"},
        {227, "Products from NCEP SREF processing"},{228, "NAEFS Products from joined NCEP, CMC global ensembles"},{229, "Downscaled GFS from NAM eXtension"},
        {230, "WRF-EM model, generic resolution (Used in various runs) EM - Eulerian Mass-core (NCAR - aka Advanced Research WRF)"},{231, "NEMS GFS Aerosol Component"},
        {232, "UnRestricted Mesoscale Analysis (URMA)"},{233, "WAM (Whole Atmosphere Model)"},{234, "Ice Concentration Analysis"},{235, "Western North Atlantic Regional Wave Model"},
        {236, "Alaska Waters Regional Wave Model"},{237, "North Atlantic Hurricane Wave Model"},{238, "Eastern North Pacific Regional Wave Model"},{239, "North Pacific Hurricane Wave Model"},
        {240, "Sea Ice Forecast Model"},{241, "Lake Ice Forecast Model"},{242, "Global Ocean Forecast Model"},{243, "Global Ocean Data Analysis System (GODAS)"},
        {244, "Merge of fields from the RUC, NAM, and Spectral Model"},{245, "Great Lakes Wave Model"},{246, "High Resolution Ensemble Forecast (HREF)"},{247, "Great Lakes Short Range Wave Model"},
        {248, "Rapid Refresh Forecast System (RRFS)"},{249, "Hurricane Analysis and Forecast System (HAFS)"},{254, "North American Regional Reanalysis (NARR)"},{255, "CPC Automated Product"}
    };
	s_pid = processIdNumbers[pid];

	calculated_latitude = latitude_2 / 1000;
	if (latitude_1 == 128)
	{
		calculated_latitude *= -1;
	}

	calculated_longtitude = longtitude_2 / 1000;
	if (longtitude_1 == 128)
	{
		calculated_longtitude *= -1;
	}

	if (resAndCompFlags == 0)
	{
		s_resAndCompFlags = "Direction increments not given and Earth assumed spherical";
	}
	else if (resAndCompFlags == 64)
	{
		s_resAndCompFlags = "Direction increments not given and Earth assumed oblate spheroid";
	}
	else if (resAndCompFlags == 128)
	{
		s_resAndCompFlags = "Direction increments given and Earth assumed spherical";
	}
	else if (resAndCompFlags == 192)
	{
		s_resAndCompFlags = "Direction increments given and Earth assumed oblate spheroid";
	}
	
	calculatedLatitudeLastPoint = latitudeLastPoint / 1000;
	calculatedLongtitudeLastPoint = longtitudeLastPoint / 1000;

	if (longtitudalDirIncrement == 65535)
	{
		calculatedLongtitudalDirIncrement = -1;
	}
	else
	{
		calculatedLongtitudalDirIncrement = calculated_latitude;
	}

	if (latitudalDirIncrement == 65535)
	{
		calculatedLatitudalDirIncrement = -1;
	}
	else
	{
		calculatedLatitudalDirIncrement = calculated_longtitude;
	}

	if (scanningModeFlags == 0)
	{
		s_scanningModeFlags = "Points scan in +i and -j. FORTRAN:(I,J)";
	}
	else if (scanningModeFlags == 32)
	{
		s_scanningModeFlags = "Points scan in +i and +j. FORTRAN:(J,I)";
	}
	else if (scanningModeFlags == 64)
	{
		s_scanningModeFlags = "Points scan in +i and +j. FORTRAN:(I,J)";
	}
	else if (scanningModeFlags == 96)
	{
		s_scanningModeFlags = "Points scan in +i and +j. FORTRAN:(J,I)";
	}
	else if (scanningModeFlags == 128)
	{
		s_scanningModeFlags = "Points scan in -i and -j. FORTRAN:(I,J)";
	}
	else if (scanningModeFlags == 160)
	{
		s_scanningModeFlags = "Points scan in -i and -j. FORTRAN:(J,I)";
	}
	else if (scanningModeFlags == 192)
	{
		s_scanningModeFlags = "Points scan in -i and +j. FORTRAN:(I,J)";
	}
	else if (scanningModeFlags == 224)
	{
		s_scanningModeFlags = "Points scan in -i and +j. FORTRAN:(J,I)";
	}
}

void Decode::printSection0() const
{
	std::cout << "Coding name: " << codingName << "\n";
	std::cout << "==========================\n== General Message Info ==\n==========================\n";
	std::cout << "GRIB found at pos: " << section0_start << "\n";
	std::cout << "7777 found at pos: " << fileEnd << "\n";
	std::cout << "Distance between end of grib and start of 7777: " << (fileEnd - section0_start) << "\n";
	std::cout << "Whole message length: " << messageLength << "\n";
}

void Decode::printSection1() const
{
	std::cout << "\n=====================\n== Section 1 Data  ==\n=====================\n";
	std::cout << "Section 1 length: " << section1_length << "\n";
	std::cout << "Parameter table Version: " << versionNumber << "\n";
	std::cout << "Identification of Centre: " << s_originCentre << "\n";
	std::cout << "process ID number: " << s_pid << "\n";
	std::cout << "Grid Identification: " << gridDef << "\n";
	std::cout << "GDS and BMS: " << s_flag << "\n";
	std::cout << "Unit parameters: " << parameterIndicator << "\n";
	std::cout << "Indicator of type of level or layer: " << lvlTypeIndicator << "\n";
	std::cout << "Height, pressure, etc. of the level or layer: " << height_pressure << "\n";
	std::cout << "YY/MM/DD|HH:MM : " << year << "/" << month << "/" << day << " | " << s_hour << ":" << s_minute << "\n";
	std::cout << "Time unit: " << unitTimeRange << "\n";
	std::cout << "P1 - Period of time: " << periodOfTime << "\n";
	std::cout << "P2 - Period of time: " << intervalTime << "\n";
	std::cout << "Time range indicator: " << timeRangeIndicator << "\n";
	std::cout << "Numbers for last row: " << numberIncluded << "\n";
	std::cout << "Number missing from averages or accumulation: " << numberMissing << "\n";
	std::cout << "Reference century: " << centuryReference << "\n";
	std::cout << "Identification of sub Center: " << subCentreIdentification << "\n";
	std::cout << "Decimal Scale factor: " << decimalScaleFactor << "\n";
}

void Decode::printSection2() const
{
	std::cout << "\n=====================\n== Section 2 Data  ==\n=====================\n";
	std::cout << "Section 2 length: " << section2_length << "\n";
	std::cout << "Number of Vertical Coordinates: " << numberOfVerticals << "\n";
	std::cout << "Octet number: " << location << "\n";
	std::cout << "Data representation type: " << dataRepresentationType << "\n";
	std::cout << "Numbers of points along a latitude circle: " << numOfPointsLatitude << "\n";
	std::cout << "Numbers of points along a longitude meridian: " << numOfPointsLongitude << "\n";
	std::cout << "Latitude: " << calculated_latitude << " degrees\n";
	std::cout << "Longtitude: " << calculated_longtitude << " degrees\n";
	std::cout << "Resolution and component flags: " << s_resAndCompFlags << "\n";
	std::cout << "Latitude of last grid point: " << calculatedLatitudeLastPoint << " degrees\n";
	std::cout << "Longitude of last grid point: " << calculatedLongtitudeLastPoint << " degrees\n";
	std::cout << "Longitudinal Direction Increment: " << calculatedLongtitudalDirIncrement << "\n";
	std::cout << "Latitudinal Direction Increment: " << calculatedLatitudalDirIncrement << "\n";
	std::cout << "Scanning mode flags: " << s_scanningModeFlags << "\n";

	for (int i = 0; i < listOfNumbers.size(); i++)
	{
		std::cout << "Wiersz " << i + 1 << " Punktów: " << listOfNumbers[i] << "\n";
	}
}

void Decode::printSection3() const
{
	std::cout << "\n=====================\n== Section 3 Data  ==\n=====================\n";
	std::cout << "Section 3 length: " << section3_length << "\n";
	std::cout << "Number of unused bits at end of Section 3: " << unusedBits << "\n";
	std::cout << "Table reference: " << tableReference << "\n";
}

void Decode::printSection4() const
{
	std::cout << "\n=====================\n== Section 4 Data  ==\n=====================\n";
	std::cout << "Section 4 length: " << section4_length << "\n";
	std::cout << "Flag to decode: " << flag_section4 << "\n";
	std::cout << "Binary Scale Factor: " << scaleFactor << "\n";
}

void Decode::printInfo()
{
	system("cls");
	if (check == '1')
	{
		printSection0();
		printSection1();

		if (flag == 64)
		{
			printSection3();
		}
		else if (flag == 128)
		{
			printSection2();
		}
		else if (flag == 192)
		{
			printSection2();
			printSection3();
		}
		else if (flag == 0)
		{

		}

		printSection4();

		_getch();
	}
	else
	{
		std::cout << "Nie wczytano ¿adnego pliku .grib\n";
		_getch();
	}
}

void Decode::main_menu()
{
	char choice = 0;
	while (choice != '3')
	{
		system("cls");
		std::cout << "MENU PROGRAMU DEKODUJ¥CEGO PLIKI .GRIB\n";
		std::cout << "Wybierz z jakiej funkcjonalnoœci chcia³(a)byœ skorzystaæ:\n";
		std::cout << "1. Za³aduj plik .grib\n2. Wyœwietl informacje zawarte w pliku .grib (wymaga uprzedniego za³adowania)\n3. Wyjœcie z programu\n";
		choice = _getch();

		switch(choice)
		{
		case '1':
			loadFile();
			break;

		case '2':
			printInfo();
			break;

		case '3':
			system("cls");
			std::cout << "Dziêki za skorzystanie z programu :)";
			break;

		default:
			break;
		}
	}
}