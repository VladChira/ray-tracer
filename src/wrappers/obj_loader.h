#pragma once
#include "tiny_obj_loader.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "clock_util.h"
#include "console.h"

bool LoadObj(std::string filename, tinyobj::attrib_t &attrib, std::vector<tinyobj::shape_t> &shapes, std::vector<tinyobj::material_t> &materials);

void PrintInfo(const tinyobj::attrib_t &attrib,
               const std::vector<tinyobj::shape_t> &shapes,
               const std::vector<tinyobj::material_t> &materials);
// bool TestLoadObj(const char *filename, const char *basepath = NULL, bool triangulate = true);
