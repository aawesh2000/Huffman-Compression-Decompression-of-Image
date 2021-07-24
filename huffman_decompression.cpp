#include<bits/stdc++.h>
using namespace std;


int main(int argc, char **argv)
{

	// check if any file is entered or not
	if(argc < 2)
	{
    cout << "Please input the file" << endl;
    return 0;
  	}

  //open the file by creating object of class ifstream
  ifstream file;
  file.open(argv[1], ios::in | ios::binary);


  //here third argument is the file name of the output file
  // if file name is not specified name will be same as input file
  ofstream output;
  bool flag = true;
  if(argc > 2){
    flag = false;
    output.open(argv[2], ios::out);
  }

  // this code will run only if file name is not specified
  // default file name is be same as input file
  char *end = argv[1] + strlen(argv[1]) - 5;
  if(flag && (strcmp(end, ".zzip") == 0)){
    flag = false;
    // here we replace last '.' with NULL character
    argv[1][strlen(argv[1]) - 5] = '\0';
    output.open(argv[1], ios::out);
  }



  // get length of file:
  file.seekg(0, ios::end);
  int length = file.tellg();
  file.seekg(0, ios::beg);




  // retriving code of each character

  char *data = new char[length];

  file.read(data, length);

  int dataStart = 0;

  map<int, char> lookup;
  char value;
  int code;
  for(int i = 0; i < length;){
    value = data[i];
    i += 1;
    code = *((int *)(data+i));
    i += 4;
    lookup[code] = value;
    // cout << value << ": " << toBits(code) << endl;
    if(value == '\0'){
      dataStart = i;
      break;
    }
  }


  // for(map<int, char>::iterator i = lookup.begin(); i != lookup.end(); i++){
  //    cout << i->first << ": " << i->second << endl;
  // }


  // writing to output file
  int selected = 0;
  for(int i = dataStart; i < length; i += 4){
    int current = *((int *)(data+i));
    // cout << "Got: " << toBits(current) << endl;
    for(int b = (sizeof(current)*8 - 1); b >= 0; b--){
      selected <<= 1;
      int bit = ((current >> b) & 1);
      // cout << "Bit: " << bit << endl;
      selected |= bit;
      // cout << "selected: " << selected << endl;
      if(lookup.find(selected) != lookup.end()){
        // cout << "Found character for selection: " << selected << endl;
        if(flag)
          cout << lookup[selected];
        else
          output << lookup[selected];
        selected = 0;
      }
    }
  }



}
