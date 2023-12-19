//============================================================================
// Name        : DesignByContract.h
// Author      : Serge Demeyer
// Version     : ?
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : Modified by Kars van Velzen
//============================================================================

#ifndef TOI_HELPER_H
#define TOI_HELPER_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <ctime>
#include <set>
#include <algorithm>
#include <memory>

/**
 * Namespace specifically for the helper functions
 */
namespace sos {
    /**
     * \brief Check if the given directory exists
     * @param dirname
     * @return
     */
    bool DirectoryExists(const std::string &dirname);

    /**
     * \brief Check if a File is existing
     * @param dirname
     * @return
     */
    bool FileExists(const std::string &dirname);

    /**
     * \brief Check if a file is empty
     * @param filename
     * @return
     */
    bool FileIsEmpty(const std::string &filename);

    /**
     * \brief Compare 2 files
     * @param leftFileName
     * @param rightFileName
     * @return
     */
    bool FileCompare(const std::string &leftFileName, const std::string &rightFileName);
}

#endif //TOI_HELPER_H
