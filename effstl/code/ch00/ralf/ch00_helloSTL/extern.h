#pragma once

#include <iostream>
#include <vector>
#include <list>

using namespace std;

#define SAFE_DELETE(p) if(p) { delete(p); (p) = nullptr; }