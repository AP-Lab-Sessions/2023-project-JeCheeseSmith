//
// Created by watson on 10/6/23.
//

#include "helper.h"

namespace sos {
    bool DirectoryExists(const std::string &dirname) {
        struct stat st;
        return stat(dirname.c_str(), &st) == 0;
    }

    bool FileExists(const std::string &filename) {
        struct stat st;
        if (stat(filename.c_str(), &st) != 0) return false;
        std::ifstream f(filename.c_str());
        if (f.good()) {
            f.close();
            return true;
        } else {
            f.close();
            return false;
        }
    }

    bool FileIsEmpty(const std::string &filename) {
        struct stat st;
        if (stat(filename.c_str(), &st) != 0) return true; // The File does not exist; thus it is empty
        return st.st_size == 0;
    }

    bool FileCompare(const std::string &leftFileName, const std::string &rightFileName) {
        std::ifstream leftFile, rightFile;
        char leftRead, rightRead;
        bool result;

        // Open the two files.
        leftFile.open(leftFileName.c_str());
        if (!leftFile.is_open()) {
            return false;
        }
        rightFile.open(rightFileName.c_str());
        if (!rightFile.is_open()) {
            leftFile.close();
            return false;
        }

        result = true; // files exist and are open; assume equality unless a counterexamples shows up.
        while (result && leftFile.good() && rightFile.good()) {
            leftFile.get(leftRead);
            rightFile.get(rightRead);
            result = (leftRead == rightRead);
        }
        if (result) {
            // the last read was still equal; are we at the end of both files?
            result = (!leftFile.good()) && (!rightFile.good());
        }

        leftFile.close();
        rightFile.close();
        return result;
    }
}