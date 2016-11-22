// On my honor, I have neither given nor received unauthorized aid on this assignment
#include <iostream>
#include <fstream>
#include <math.h>
#include <unordered_map>

using namespace std;

class InstCat {

	public:

	string instruction = "";
	int target = 0;
	string register1 = "";
  // base will also be called register2
  string register2 = "";
	string destination = "";
	// immediate will also double as shift amount
	string immediate = "";
	int type = 0;

	// constructor for category 1 instructions
	InstCat(string inst, int tar, string reg1, string reg2, string dest) {
		instruction = inst;
		target = tar;
		register1 = reg1;
		register2 = reg2;
		destination = dest;
		type = 1;
	}

	// constructor for category 2 instructions
	InstCat(string inst, string reg1, string reg2, string dest) {
		instruction = inst;
		register1 = reg1;
		register2 = reg2;
		destination = dest;
		type = 2;
	}

	// constructor for category 3 instructions
	InstCat(string inst, string reg1, string dest, string immed, int type) {
		instruction = inst;
		register1 = reg1;
		destination = dest;
		immediate = immed;
		type = 3;
	}

	InstCat() {}

};

bool broke = false;
unordered_map<double, InstCat> MIPSInstructions;

int evaluateBinary(string binary);
int dosComplement(string line);

int shiftLeft(int number, int amount)
{
  for (int i = 0; i < amount; i++)
  {
    number *= 2;
  }
  return number;
}

string category3opCode(string line, int address)
{
  string instruction;
  if (line.substr(3,3) == "000")
  {
    instruction = "ORI";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string immediate = line.substr(16);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    //immediate = to_string(evaluateBinary(immediate));
    if (immediate.substr(0,4) == "1111")
    {
      immediate = to_string(dosComplement(immediate));
    } else
    {
      immediate = to_string(evaluateBinary(immediate));
    }
    instruction = instruction + " "  + destination + ", " + source1 + ", #" + immediate;
		MIPSInstructions[address] = InstCat(instruction, source1, destination, immediate, 3);
  } else if (line.substr(3,3) == "001")
  {
    instruction = "XORI";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string immediate = line.substr(16);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    // immediate = to_string(evaluateBinary(immediate));
    if (immediate.substr(0,4) == "1111")
    {
      immediate = to_string(dosComplement(immediate));
    } else
    {
      immediate = to_string(evaluateBinary(immediate));
    }
    instruction = instruction + " "  + destination + ", " + source1 + ", #" + immediate;
		MIPSInstructions[address] = InstCat(instruction, source1, destination, immediate, 3);
  } else if (line.substr(3,3) == "010")
  {
    instruction = "ADDI";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string immediate = line.substr(16);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    // immediate = to_string(evaluateBinary(immediate));
    if (immediate.substr(0,4) == "1111")
    {
      immediate = to_string(dosComplement(immediate));
    } else
    {
      immediate = to_string(evaluateBinary(immediate));
    }

    instruction = instruction + " "  + destination + ", " + source1 + ", #" + immediate;
		MIPSInstructions[address] = InstCat(instruction, source1, destination, immediate, 3);
  } else if (line.substr(3,3) == "011")
  {
    instruction = "SUBI";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string immediate = line.substr(16);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    // immediate = to_string(evaluateBinary(immediate));
    if (immediate.substr(0,4) == "1111")
    {
      immediate = to_string(dosComplement(immediate));
    } else
    {
      immediate = to_string(evaluateBinary(immediate));
    }

    instruction = instruction + " "  + destination + ", " + source1 + ", #" + immediate;
		MIPSInstructions[address] = InstCat(instruction, source1, destination, immediate, 3);
  } else if (line.substr(3,3) == "100")
  {
    instruction = "ANDI";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string immediate = line.substr(16);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    // immediate = to_string(evaluateBinary(immediate));
    if (immediate.substr(0,4) == "1111")
    {
      immediate = to_string(dosComplement(immediate));
    } else
    {
      immediate = to_string(evaluateBinary(immediate));
    }
    instruction = instruction + " "  + destination + ", " + source1 + ", #" + immediate;
		MIPSInstructions[address] = InstCat(instruction, source1, destination, immediate, 3);
  } else if (line.substr(3,3) == "101")
  {
    instruction = "SRL";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string shiftAmount = line.substr(27);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    shiftAmount = to_string(evaluateBinary(shiftAmount));
    instruction = instruction + " " + destination + ", "  + source1 + ", #" + shiftAmount;
		MIPSInstructions[address] = InstCat(instruction, source1, destination, shiftAmount, 3);
  } else if (line.substr(3,3) == "110")
  {
    instruction = "SRA";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string shiftAmount = line.substr(27);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    shiftAmount = to_string(evaluateBinary(shiftAmount));
    instruction = instruction + " " + destination + ", "  + source1 + ", #" + shiftAmount;
		MIPSInstructions[address] = InstCat(instruction, source1, destination, shiftAmount, 3);
  } else if (line.substr(3,3) == "111")
  {
    instruction = "SLL";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string shiftAmount = line.substr(27);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    shiftAmount = to_string(evaluateBinary(shiftAmount));
    instruction = instruction + " " + destination + ", "  + source1 + ", #" + shiftAmount;
		MIPSInstructions[address] = InstCat(instruction, source1, destination, shiftAmount, 3);
  }
  return instruction;
}

string category2opCode(string line, int address)
{
  string instruction;
  if (line.substr(3,3) == "000")
  {
    instruction = "XOR";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string source2 = line.substr(16,5);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    source2 = "R" + to_string(evaluateBinary(source2));
    instruction = instruction + " " + destination + ", " + source1 + ", " + source2;
		MIPSInstructions[address] = InstCat(instruction, source1, source2, destination);
  } else if (line.substr(3,3) == "001")
  {
    instruction = "MUL";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string source2 = line.substr(16,5);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    source2 = "R" + to_string(evaluateBinary(source2));
    instruction = instruction + " " + destination + ", " + source1 + ", " + source2;
		MIPSInstructions[address] = InstCat(instruction, source1, source2, destination);
  } else if (line.substr(3,3) == "010")
  {
    instruction = "ADD";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string source2 = line.substr(16,5);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    source2 = "R" + to_string(evaluateBinary(source2));
    instruction = instruction + " " + destination + ", " + source1 + ", " + source2;
		MIPSInstructions[address] = InstCat(instruction, source1, source2, destination);
  } else if (line.substr(3,3) == "011")
  {
    instruction = "SUB";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string source2 = line.substr(16,5);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    source2 = "R" + to_string(evaluateBinary(source2));
    instruction = instruction + " " + destination + ", " + source1 + ", " + source2;
		MIPSInstructions[address] = InstCat(instruction, source1, source2, destination);
  } else if (line.substr(3,3) == "100")
  {
    instruction = "AND";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string source2 = line.substr(16,5);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    source2 = "R" + to_string(evaluateBinary(source2));
    instruction = instruction + " " + destination + ", " + source1 + ", " + source2;
		MIPSInstructions[address] = InstCat(instruction, source1, source2, destination);
  } else if (line.substr(3,3) == "101")
  {
    instruction = "OR";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string source2 = line.substr(16,5);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    source2 = "R" + to_string(evaluateBinary(source2));
    instruction = instruction + " " + destination + ", " + source1 + ", " + source2;
		MIPSInstructions[address] = InstCat(instruction, source1, source2, destination);
  } else if (line.substr(3,3) == "110")
  {
    instruction = "ADDU";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string source2 = line.substr(16,5);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    source2 = "R" + to_string(evaluateBinary(source2));
    instruction = instruction + " " + destination + ", " + source1 + ", " + source2;
		MIPSInstructions[address] = InstCat(instruction, source1, source2, destination);
  } else if (line.substr(3,3) == "111")
  {
    instruction = "SUBU";
    string destination = line.substr(6,5);
    string source1 = line.substr(11,5);
    string source2 = line.substr(16,5);
    destination = "R" + to_string(evaluateBinary(destination));
    source1 = "R" + to_string(evaluateBinary(source1));
    source2 = "R" + to_string(evaluateBinary(source2));
    instruction = instruction + " " + destination + ", " + source1 + ", " + source2;
		MIPSInstructions[address] = InstCat(instruction, source1, source2, destination);
  }
  return instruction;
}

string category1opCode(string line, int address)
{
  string instruction;
  if (line.substr(3,3) == "000")
  {
    instruction = "NOP";
  } else if (line.substr(3,3) == "001")
  {
    instruction = "J";
    string jumpTarget = line.substr(6);
    int jumpAdd = evaluateBinary(jumpTarget);
    jumpAdd = shiftLeft(jumpAdd, 2);
    instruction = instruction + " #" + to_string(jumpAdd);
  } else if (line.substr(3,3) == "010")
  {
    instruction = "BEQ";
    string register1 = line.substr(6,5);
    string register2 = line.substr(11,5);
    string beqOffset = line.substr(16);
    register1 = "R" + to_string(evaluateBinary(register1));
    register2 = "R" + to_string(evaluateBinary(register2));
    beqOffset = to_string(evaluateBinary(beqOffset));
    instruction = instruction + " " + register1 + ", " + register2 + ", " + "#" + beqOffset;
  } else if (line.substr(3,3) == "011")
  {
    instruction = "BNE";
    string register1 = line.substr(6,5);
    string register2 = line.substr(11,5);
    string bneOffset = line.substr(16);
    register1 = "R" + to_string(evaluateBinary(register1));
    register2 = "R" + to_string(evaluateBinary(register2));
    bneOffset = to_string(evaluateBinary(bneOffset));
    instruction = instruction + " " + register1 + ", " + register2 + ", " + "#" + bneOffset;
  } else if (line.substr(3,3) == "100")
  {
    instruction = "BGTZ";
    string register1 = line.substr(6,5);
    string register2 = "0";
    string beqOffset = line.substr(16);
    register1 = "R" + to_string(evaluateBinary(register1));
    register2 = "R" + register2;
    beqOffset = to_string(dosComplement(beqOffset));
    instruction = instruction + " " + register1 + ", #" + beqOffset;
  } else if (line.substr(3,3) == "101")
  {
    instruction = "SW";
    string register1 = line.substr(11,5);
    string base = line.substr(6,5);
    string offset = line.substr(16);
    register1 = "R" + to_string(evaluateBinary(register1));
    base = "R" + to_string(evaluateBinary(base));
    if (offset.substr(0,4) == "1111")
    {
      offset = to_string(dosComplement(offset));
    } else
    {
      offset = to_string(evaluateBinary(offset));
    }
    instruction = instruction + " " + register1 + ", " + offset + "(" + base + ")";
  } else if (line.substr(3,3) == "110")
  {
    instruction = "LW";
    string register1 = line.substr(11,5);
    string base = line.substr(6,5);
    string offset = line.substr(16);
    register1 = "R" + to_string(evaluateBinary(register1));
    base = "R" + to_string(evaluateBinary(base));
    if (offset.substr(0,4) == "1111")
    {
      offset = to_string(dosComplement(offset));
    } else
    {
      offset = to_string(evaluateBinary(offset));
    }
    instruction = instruction + " " + register1 + ", " + offset + "(" + base + ")";
  } else if (line.substr(3,3) == "111")
  {
    instruction = "BREAK";
    broke = true;
  }
  return instruction;
}

string determineCategory(string line, int address)
{
  string instruction;
  if (line.substr (0,3) == "001")
  {
    instruction = category1opCode(line, address);
  } else if (line.substr(0,3) == "010")
  {
    instruction = category2opCode(line, address);
  } else if (line.substr(0,3) == "100")
  {
    instruction = category3opCode(line, address);
  }
  return instruction;
}

int evaluateBinary(string binary)
{
  int value = 0;
  for (int i = 0; i < binary.length(); i++)
  {
    if (binary.at(i) == '1')
    {
      int index = binary.length() - i - 1;
      value += pow(2, index);
    }
  }
  return value;
}

int dosComplement(string line)
{
  // flip all of the bits in the twos complement binary
  for (int i = 0; i < line.length(); i++)
  {
    if (line[i] == '0')
    {
      line[i] = '1';
    } else if (line[i] == '1')
    {
      line[i] = '0';
    }
  }

  // cout << "flipped bits: " << line << "\n";

  // starts on right side of string and converts all '1' to '0' until iteration reaches a '0'
  // then converts that '0' to a '1' and breaks
  int j;
  for (j = line.length() - 1; j >= 0; j--)
  {
    if (line[j] == '1')
            line[j] = '0';
    else
    {
        line[j] = '1';
        break;
    }
  }

  // cout << "added one: " << line << "\n";

  int dosValue = 0 - evaluateBinary(line);
  return dosValue;
}

int main(int argc, char *argv[])
{
  // std::cout << "There are " << argc << " arguments:\n";

  // Loop through each argument and print its number and value
  // for (int count=0; count < argc; ++count)
  //    std::cout << count << " " << argv[count] << '\n';

  string filename = argv[1];

  string line;
  ifstream inputfile (filename);
  ofstream disassemblyfile ("disassembly.txt");
  int address = 60;
  if (inputfile.is_open() && disassemblyfile.is_open())
  {
    while ( getline (inputfile,line) )
    {
      address += 4;
      if (!broke)
      {
        string instruction = determineCategory(line, address);
        disassemblyfile << line << "\t" << address << "\t" << instruction << "\n";
      } else
      {
        int dosValue = dosComplement(line);
        disassemblyfile << line << "\t" << address << "\t" << dosValue << "\n";
      }
    }
    disassemblyfile.close();
    inputfile.close();
  } else
  {
    cout << "Unable to open file";
  }

  return 0;
}
