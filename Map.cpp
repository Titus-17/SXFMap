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
		return (1);
	}

	// Checking first 2 data fields.															
	infile.read((char*)(&m_passport.id),	sizeof(long));	// Checking accordance of id to 
	if (m_passport.id != 4610131){							// SXF (0x00465853 or 4610131).
		cerr << "Error! Wrong id.\n";
		return (2);
	}

	infile.read((char*)(&m_passport.passportLength),sizeof(long));	// Checking passport 
	if (m_passport.passportLength != 400){							// length (v4.0 2008 has
		cerr << "Error! Wrong passport length. Check version.\n";	// 400 bytes)
		return (3);
	}
	
	infile.seekg(0); 
	infile.read((char*)(&m_passport), m_passport.passportLength);
	infile.close();

	cout << filename << " was read" << "\n";
	return 0;
}


int cMap::writePassportLog(char* filename)
{
	ofstream outfile;
	outfile.open(filename);
	if (!outfile){	
		cerr << "Unable to write log file!\n";	
		outfile.close();
		return(1);
	}

	outfile << 
	// General data
	 	hex	<< "id = "					<< m_passport.id					<< "\n"
	<<	dec	<< "passportLength = "		<< m_passport.passportLength		<< "\n"
	<<	hex	<< "edition = "				<< m_passport.edition				<< "\n"
	<<	dec	<< "checkSum = "			<< m_passport.checkSum				<< "\n"
			<< "creationgDate = "		<< m_passport.creationgDate		 	<< "\n"
			<< "nomenclature = "		<< m_passport.nomenclature			<< "\n"
			<< "scale = "				<< m_passport.scale					<< "\n"
			<< "sheetName = "			<< m_passport.sheetName				<< "\n\n"
	// Info flags
			<< "flag_status = "			<< m_passport.flag_status			<< "\n"
			<< "flag_precision = "		<< m_passport.flag_projection		<< "\n"
			<< "flag_realCoordinate = "	<< m_passport.flag_realCoordinate	<< "\n"
			<< "flag_codingType = "		<< m_passport.flag_codingType		<< "\n"
			<< "flag_generalTable = "	<< m_passport.flag_generalTable		<< "\n"
			<< "flag_signature = "		<< m_passport.flag_signature		<< "\n"
			<< "flag_precision = "		<< m_passport.flag_precision		<< "\n"
			<< "flag_specSort = "		<< m_passport.flag_specSort			<< "\n"
			<< "flag_reserveFlag = "	<< m_passport.flag_reserve			<< "\n\n"

			<< "classifier = "			<< m_passport.classifier			<< "\n\n"
	// Rectangular coordinates of sheet corners
			<< "XSouthWestCorner = "	 << m_passport.XSouthWestCorner		<< "\n"
			<< "YSouthWestCorner = "	 << m_passport.YSouthWestCorner		<< "\n"
			<< "XNorthWestCorner = "	 << m_passport.XNorthWestCorner		<< "\n"
			<< "YNorthWestCorner = "	 << m_passport.YNorthWestCorner		<< "\n"
			<< "XNorthEastCorner = "	 << m_passport.XNorthEastCorner		<< "\n"
			<< "YNorthEastCorner = "	 << m_passport.YNorthEastCorner		<< "\n"
			<< "XSouthEastCorner = "	 << m_passport.XSouthEastCorner		<< "\n"
			<< "YSouthEastCorner = "	 << m_passport.YSouthEastCorner		<< "\n\n"
	// Geodetic coordinates of sheet corners
			<< "BSouthWestCorner = "	<< m_passport.BSouthWestCorner		<< "\n"
			<< "LSouthWestCorner = "	<< m_passport.LSouthWestCorner		<< "\n"
			<< "BNorthWestCorner = "	<< m_passport.BNorthWestCorner		<< "\n"
			<< "LNorthWestCorner = "	<< m_passport.LNorthWestCorner		<< "\n"
			<< "BNorthEastCorner = "	<< m_passport.BNorthEastCorner		<< "\n"
			<< "LNorthEastCorner = "	<< m_passport.LNorthEastCorner		<< "\n"
			<< "BSouthEastCorner = "	<< m_passport.BSouthEastCorner		<< "\n"
			<< "LSouthEastCorner = "	<< m_passport.LSouthEastCorner		<< "\n\n"
	// Mathematical basis of sheet
			<< "ellipsoidType = "		<< m_passport.ellipsoidType			<< "\n"
			<< "elevationSystem = "		<< m_passport.elevationSystem		<< "\n"
			<< "materialProjection = "	<< m_passport.materialProjection	<< "\n"
			<< "coordinateSystem = "	<< m_passport.coordinateSystem		<< "\n"
			<< "planeUnit = "			<< m_passport.planeUnit				<< "\n"
			<< "heightUnit = "			<< m_passport.heightUnit			<< "\n"
			<< "frameType = "			<< m_passport.frameType				<< "\n"
			<< "mapType = "				<< m_passport.mapType				<< "\n\n"
	// Reference data on source material
			<< "updateDate = "			<< m_passport.updateDate			<< "\n"
			<< "materialKind = "		<< m_passport.materialKind			<< "\n"
			<< "materialType = "		<< m_passport.materialType			<< "\n"
			<< "reserveRef1 = "			<< m_passport.reserveRef1			<< "\n"
			<< "magneticDeclination = "	<< m_passport.magneticDeclination	<< "\n"
			<< "avgMeridianConv = "		<< m_passport.avgMeridianConv		<< "\n"
			<< "magneticChange = "		<< m_passport.magneticChange		<< "\n"
			<< "declinationUpdt = "		<< m_passport.declinationUpdt		<< "\n"
			<< "reserveRef2 = "			<< m_passport.reserveRef2			<< "\n"
			<< "reliefSection = "		<< m_passport.reliefSection			<< "\n"
			<< "reserve = "				<< m_passport.reserve				<< "\n"
			<< "deviceResolution = "	<< m_passport.deviceResolution		<< "\n\n"
	// Location of the frame on the device
			<< "X_SW_FrameLoacation = "	<< m_passport.X_SW_FrameLoacation	<< "\n"
			<< "Y_SW_FrameLoacation = "	<< m_passport.Y_SW_FrameLoacation	<< "\n"
			<< "X_NW_FrameLoacation = "	<< m_passport.X_NW_FrameLoacation	<< "\n"
			<< "Y_NW_FrameLoacation = "	<< m_passport.Y_NW_FrameLoacation	<< "\n"
			<< "X_NE_FrameLoacation = "	<< m_passport.X_NE_FrameLoacation	<< "\n"
			<< "Y_NE_FrameLoacation = "	<< m_passport.Y_NE_FrameLoacation	<< "\n"
			<< "X_SE_FrameLoacation = "	<< m_passport.X_SE_FrameLoacation	<< "\n"
			<< "Y_SE_FrameLoacation = "	<< m_passport.Y_SE_FrameLoacation	<< "\n\n"
	// Reference data on the projection of the source material	
			<< "firstMainParallel = "	<< m_passport.firstMainParallel		<< "\n"
			<< "secondMainParallel = "	<< m_passport.secondMainParallel	<< "\n"
			<< "axialMeridian = "		<< m_passport.axialMeridian			<< "\n"
			<< "mainPointParallel = "	<< m_passport.mainPointParallel		<< "\n"
			<< "poleLatitude = "		<< m_passport.poleLatitude			<< "\n"
			<< "poleLongitude = "		<< m_passport.poleLongitude			<< "\n";
	outfile.close();
	cout << "Passport was recored in " << filename << "\n";
	return 0;
}
