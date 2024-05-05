#ifndef _DECODE_HPP_
#define _DECODE_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <map>
#include <vector>

class Decode
{

public:
	/// <summary>
	///	Program loads in the GRIB1 file that the user wants.
	/// Returns all the important information from said file
	/// </summary>

	// Constructor that sets all the starting values for variables
	Decode();

	// Method used for printing out the menu that user can navigate throught (load file and print the data from it)
	void main_menu();

private:
	// Variable that contains the name of the file (automatically adds .grib to the end of the inputed name)
	std::string fileName;

	// Variable that checks if the file has been loaded by the user to avoid printing out not loaded contents
	char check;

	// SECTION 0 variables
	uint32_t section0_start, fileEnd, messageLength, editionNumber;
	std::string codingName;

	// SECTION 1 variables
	uint32_t section1_start, section1_length, versionNumber, originCentre, pid, gridDef, flag, parameterIndicator, lvlTypeIndicator, height_pressure, year,
	month, day, hour, minute, unitTimeRange, periodOfTime, intervalTime, timeRangeIndicator, numberIncluded, numberMissing, centuryReference,
	subCentreIdentification, decimalScaleFactor;

	// SECTION 2 variables
	uint32_t section2_start, section2_length, numberOfVerticals, location, dataRepresentationType, numOfPointsLatitude, numOfPointsLongitude, latitude_1, latitude_2,
	longtitude_1, longtitude_2, resAndCompFlags, latitudeLastPoint, longtitudeLastPoint, scanningModeFlags, longtitudalDirIncrement, latitudalDirIncrement;
	std::vector<uint16_t> listOfNumbers;

	// SECTION 3 variables
	uint32_t section3_start, section3_length, unusedBits, tableReference;

	// SECTION 4 variables
	uint32_t section4_start, section4_length, flag_section4, scaleFactor;

	// Variables values set based off of data from the file
	std::string s_flag, s_originCentre, s_pid, s_hour, s_minute, s_resAndCompFlags, s_scanningModeFlags;
	int16_t calculated_latitude, calculated_longtitude, calculatedLatitudeLastPoint, calculatedLongtitudeLastPoint, calculatedLongtitudalDirIncrement,
	calculatedLatitudalDirIncrement;

	// Methods that load 3, 2 and 1 bytes from the file, starting from a said position in the file
	uint32_t readThreeBytes(uint32_t mPos) const;
	uint16_t readTwoBytes(uint32_t mPos) const;
	uint8_t readOneByte(uint32_t mPos) const;

	// Methods used for printing out all the important information from file's sections
	void loadFile();
	void printInfo();
	void loadSection0();
	void loadSection1();
	void loadSection2();
	void loadSection3();
	void loadSection4();

	// Method used for setting text values based off of data from the file
	void setProperValues();

	// Methods used for printing the data from all the sections
	void printSection0() const;
	void printSection1() const;
	void printSection2() const;
	void printSection3() const;
	void printSection4() const;

};

#endif // _DECODE_HPP_