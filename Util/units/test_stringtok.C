/***************************************************************************
 *
 *   Copyright (C) 2023 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "stringtok.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main ()
{
  string text = "A,B,C,D";
  vector<string> tokens;
  string sub;
  while ( (sub = stringtok (text, ",")) != "")
    tokens.push_back(sub);

  cerr << "tokens.size=" << tokens.size() << endl;

  return 0;
}
