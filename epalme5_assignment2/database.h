/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/
#pragma once

#include "model.h"
#include "vertex.h"

#include <vector>
#include <memory>
using namespace std;

extern unique_ptr<Model> gPyramid;
extern unique_ptr<Model> gRoad;
extern unique_ptr<Model> gBackWall;


void initModels();