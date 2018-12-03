//Map.cpp
#include <iostream>
#include <fstream>		// read()

#include "Map.h"

using namespace std;

cMap::cMap(){}

int cMap::readPassport(char* filename)
{
	ifstream infile;
	infile.open(filename, ios::binary);
	if (!infile){
		cerr << "Unable to read SXF!\n";
		infile.close();
		return (-1);
	}

	// Checking first 2 data fields.															
	infile.read((char*)(&passport.m_id),	sizeof(long));	// Checking accordance of id to 
	if (passport.m_id != 4610131){							// SXF (0x00465853 or 4610131).
		cerr << "Error! Wrong id.\n";
		return (-1);
	}

	infile.read((char*)(&passport.m_passportLength),sizeof(long));	// Checking passport 
	if (passport.m_passportLength != 400){							// length (v4.0 2008 has
		cerr << "Error! Wrong passport lenght. Check version.\n";	// 400 bytes)
		return (-1);
	}

	
	infile.seekg(0);

	infile.read((char*)(&passport), passport.m_passportLength);
	writePassportLog("PassportLog.txt");

	infile.close();
	cout << filename << " was read" << "\n";
	return 0;
}


int cMap::writePassportLog(char* filename)
{

	// General data
	ofstream outfile;
	outfile.open(filename);
	if (!outfile){
		cerr << "Unable to write log file!\n";
		outfile.close();
		return(-1);
	}

	outfile << 
		hex	<< "m_id = "					<< passport.m_id					<< "\n"
	<<	dec	<< "m_passportLength = "		<< passport.m_passportLength 		<< "\n"
	<<	hex	<< "m_edition = "				<< passport.m_edition 				<< "\n"
	<<	dec	<< "m_checkSum = " 				<< passport.m_checkSum 				<< "\n"
	 		<< "m_creationgDate = "			<< passport.m_creationgDate 		<< "\n"
	 		<< "m_nomenclature = "			<< passport.m_nomenclature 			<< "\n"
	 		<< "m_scale = "					<< passport.m_scale 				<< "\n"
	 		<< "m_sheetName = "				<< passport.m_sheetName 			<< "\n\n"
	// Info flags
			<< "m_flag_status = "			<< passport.m_flag_status			<< "\n"
			<< "m_flag_precision = "		<< passport.m_flag_projection		<< "\n"
			<< "m_flag_realCoordinate = "	<< passport.m_flag_realCoordinate	<< "\n"
			<< "m_flag_codingType = "		<< passport.m_flag_codingType		<< "\n"
			<< "m_flag_generalTable = "		<< passport.m_flag_generalTable		<< "\n"
			<< "m_flag_signature = "		<< passport.m_flag_signature		<< "\n"
			<< "m_flag_precision = "		<< passport.m_flag_precision		<< "\n"
			<< "m_flag_specSort = "			<< passport.m_flag_specSort			<< "\n"
			<< "m_flag_reserveFlag = "		<< passport.m_flag_reserve			<< "\n\n"

			<< "m_classifier = "			<< passport.m_classifier 			<< "\n\n"
	// Rectangular coordinates of sheet corners
			<< "m_XSouthWestCorner = " 		<< passport.m_XSouthWestCorner 		<< "\n"
			<< "m_YSouthWestCorner = " 		<< passport.m_YSouthWestCorner 		<< "\n"
			<< "m_XNorthWestCorner = " 		<< passport.m_XNorthWestCorner 		<< "\n"
			<< "m_YNorthWestCorner = " 		<< passport.m_YNorthWestCorner 		<< "\n"
			<< "m_XNorthEastCorner = " 		<< passport.m_XNorthEastCorner 		<< "\n"
			<< "m_YNorthEastCorner = " 		<< passport.m_YNorthEastCorner 		<< "\n"
			<< "m_XSouthEastCorner = " 		<< passport.m_XSouthEastCorner 		<< "\n"
			<< "m_YSouthEastCorner = " 		<< passport.m_YSouthEastCorner 		<< "\n\n"
	// Geodetic coordinates of sheet corners
			<< "m_BSouthWestCorner = "		<< passport.m_BSouthWestCorner		<< "\n"
			<< "m_LSouthWestCorner = "		<< passport.m_LSouthWestCorner		<< "\n"
			<< "m_BNorthWestCorner = "		<< passport.m_BNorthWestCorner		<< "\n"
			<< "m_LNorthWestCorner = "		<< passport.m_LNorthWestCorner		<< "\n"
			<< "m_BNorthEastCorner = "		<< passport.m_BNorthEastCorner		<< "\n"
			<< "m_LNorthEastCorner = "		<< passport.m_LNorthEastCorner		<< "\n"
			<< "m_BSouthEastCorner = "		<< passport.m_BSouthEastCorner		<< "\n"
			<< "m_LSouthEastCorner = "		<< passport.m_LSouthEastCorner		<< "\n\n"
	// Mathematical basis of sheet
			<< "m_ellipsoidType = "			<< passport.m_ellipsoidType			<< "\n"
			<< "m_elevationSystem = "		<< passport.m_elevationSystem		<< "\n"
			<< "m_materialProjection = "	<< passport.m_materialProjection	<< "\n"
			<< "m_coordinateSystem = "		<< passport.m_coordinateSystem		<< "\n"
			<< "m_planeUnit = "				<< passport.m_planeUnit				<< "\n"
			<< "m_heightUnit = "			<< passport.m_heightUnit			<< "\n"
			<< "m_frameType = "				<< passport.m_frameType				<< "\n"
			<< "m_mapType = "				<< passport.m_mapType				<< "\n\n"
	// Reference data on source material
			<< "m_updateDate = "			<< passport.m_updateDate			<< "\n"
			<< "m_materialKind = "			<< passport.m_materialKind			<< "\n"
			<< "m_materialType = "			<< passport.m_materialType			<< "\n"
			<< "m_reserveRef1 = "			<< passport.m_reserveRef1			<< "\n"
			<< "m_magneticDeclination = "	<< passport.m_magneticDeclination	<< "\n"
			<< "m_avgMeridianConv = "		<< passport.m_avgMeridianConv		<< "\n"
			<< "m_magneticChange = "		<< passport.m_magneticChange		<< "\n"
			<< "m_declinationUpdt = "		<< passport.m_declinationUpdt		<< "\n"
			<< "m_reserveRef2 = "			<< passport.m_reserveRef2			<< "\n"
			<< "m_reliefSection = "			<< passport.m_reliefSection			<< "\n"
			<< "m_reserve = "				<< passport.m_reserve				<< "\n"
			<< "m_deviceResolution = "		<< passport.m_deviceResolution		<< "\n\n"
	// Location of the frame on the device
			<< "m_X_SW_FrameLoacation = "	<< passport.m_X_SW_FrameLoacation	<< "\n"
			<< "m_Y_SW_FrameLoacation = "	<< passport.m_Y_SW_FrameLoacation	<< "\n"
			<< "m_X_NW_FrameLoacation = "	<< passport.m_X_NW_FrameLoacation	<< "\n"
			<< "m_Y_NW_FrameLoacation = "	<< passport.m_Y_NW_FrameLoacation	<< "\n"
			<< "m_X_NE_FrameLoacation = "	<< passport.m_X_NE_FrameLoacation	<< "\n"
			<< "m_Y_NE_FrameLoacation = "	<< passport.m_Y_NE_FrameLoacation	<< "\n"
			<< "m_X_SE_FrameLoacation = "	<< passport.m_X_SE_FrameLoacation	<< "\n"
			<< "m_Y_SE_FrameLoacation = "	<< passport.m_Y_SE_FrameLoacation	<< "\n\n"
	// Reference data on the projection of the source material	
			<< "m_firstMainParallel = "		<< passport.m_firstMainParallel		<< "\n"
			<< "m_secondMainParallel = "	<< passport.m_secondMainParallel	<< "\n"
			<< "m_axialMeridian = "			<< passport.m_axialMeridian			<< "\n"
			<< "m_mainPointParallel = "		<< passport.m_mainPointParallel		<< "\n"
			<< "m_poleLatitude = "			<< passport.m_poleLatitude			<< "\n"
			<< "m_poleLongitude = "			<< passport.m_poleLongitude			<< "\n";
	outfile.close();
	cout << "Passport was recored in " << filename << "\n";
	return 0;
}
