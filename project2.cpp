// On my honor, I have neither given nor received unauthorized aid on this assignment
#include <iostream>
#include <fstream>
#include <math.h>
#include <unordered_map>
#include <string>
#include <sstream>

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

	// constructor for instructions
	InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat) {
		instruction = inst;
		target = tar;
		register1 = reg1;
		register2 = reg2;
		destination = dest;
		immediate = immed;
		type = cat;
	}

	InstCat() {}

};

bool broke = false;
bool brokenAgain = false;
unordered_map<double, InstCat> MIPSInstructions;
// Create register arrays
int registers[32] = {0};
// Create data arrays
int data[16] = {0};
int firstData;
stringstream ss;

int evaluateBinary(string binary);
int dosComplement(string line);

string twoComplement(string binaryString)
{
	// flip all of the bits in the twos complement binary
  for (int i = 0; i < binaryString.length(); i++)
  {
    if (binaryString[i] == '0')
    {
      binaryString[i] = '1';
    } else if (binaryString[i] == '1')
    {
      binaryString[i] = '0';
    }
  }

	// starts on right side of string and converts all '1' to '0' until iteration reaches a '0'
  // then converts that '0' to a '1' and breaks
  int j;
  for (j = binaryString.length() - 1; j >= 0; j--)
  {
    if (binaryString[j] == '1')
            binaryString[j] = '0';
    else
    {
        binaryString[j] = '1';
        break;
    }
  }

	return binaryString;

}

string signExtend()
{
	string binary = "";
	ss >> binary;
	//cout << "this is what is read from the stringstream: " << binary << "\n";
	if (binary.size() < 16)
	{
		int amountOfBits = 16 - binary.size();
		if (binary[0] == '0')
		{
			for (int i = 0; i < amountOfBits; i++)
			{
				binary = "1" + binary;
			}
		} else if (binary[0] == '1')
		{
			for (int i = 0; i < amountOfBits; i++)
			{
				binary = "0" + binary;
			}
		}

	} else if (binary.size() > 16)
	{
		int length = binary.size();
		int difference = length - 16;
		binary = binary.substr(difference);
	}
	binary = twoComplement(binary);
	return binary;
}

string binaryString = "";

void binary(int number)
{
	bool negative = false;
	int remainder;

	if (number < 0) {
		number = 0 - number;
		negative = true;
	}

	if(number <= 1)
	{
		binaryString = binaryString + to_string(number);
		//ss << number;
		return;
	}

	remainder = number%2;
	binary(number >> 1);
	binaryString = binaryString + to_string(remainder);
	//ss << remainder;
	ss.str( binaryString );
	//ss << binaryString;
	//cout << "the binary string before signExtend: " << binaryString << "\n";
}

int shiftLeft(int number, int shiftAmount)
{

  for (int i = 0; i < shiftAmount; i++)
  {
    number *= 2;
  }
  return number;
}

int shiftRight(int number, int shiftAmount)
{
	double actual = (double)number;
	for (int i = 0; i < shiftAmount; i++)
	{
		actual = actual / 2;
	}
	number = (int)round(actual);
	return number;
}

string ANDcompare(string binary1, string binary2)
{
	string andString = "";
	for (int i = 0; i < 16; i++)
	{
		if (binary1.at(i) == '1' && binary2.at(i) == '1')
		{
			andString = andString + "1";
		} else
		{
			andString = andString + "0";
		}
	}
	return andString;
}

string ORcompare(string binary1, string binary2)
{
	string orString = "";
	for (int i = 0; i < 16; i++)
	{
		if (binary1.at(i) == '1' || binary2.at(i) == '1')
		{
			orString = orString + "1";
		} else
		{
			orString = orString + "0";
		}
	}
	return orString;
}

string XORcompare(string binary1, string binary2)
{
	string xorString = "";
	for (int i = 0; i < 16; i++)
	{
		if (binary1.at(i) == '1' && binary2.at(i) == '0')
		{
			xorString = xorString + "1";
		} else if (binary1.at(i) == '0' && binary2.at(i) == '1')
		{
			xorString = xorString + "1";
		} else
		{
			xorString = xorString + "0";
		}
	}
	return xorString;
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, "", destination, immediate, 3);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, "", destination, immediate, 3);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, "", destination, immediate, 3);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, "", destination, immediate, 3);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, "", destination, immediate, 3);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, "", destination, shiftAmount, 3);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, "", destination, shiftAmount, 3);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, "", destination, shiftAmount, 3);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, source2, destination, "", 2);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, source2, destination, "", 2);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, source2, destination, "", 2);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, source2, destination, "", 2);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, source2, destination, "", 2);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, source2, destination, "", 2);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, source2, destination, "", 2);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, source1, source2, destination, "", 2);
  }
  return instruction;
}

string category1opCode(string line, int address)
{
  string instruction;
  if (line.substr(3,3) == "000")
  {
    instruction = "NOP";
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, "", "", "", "", 1);
  } else if (line.substr(3,3) == "001")
  {
    instruction = "J";
    string jumpTarget = line.substr(6);
    int jumpAdd = evaluateBinary(jumpTarget);
    jumpAdd = shiftLeft(jumpAdd, 2);
    instruction = instruction + " #" + to_string(jumpAdd);
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, jumpAdd, "", "", "", "", 1);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, stoi(beqOffset), register1, register2, "", "", 1);
  } else if (line.substr(3,3) == "011")
  {
    instruction = "BNE";
    string register1 = line.substr(6,5);
    string register2 = line.substr(11,5);
    string bneOffset = line.substr(16);
    register1 = "R" + to_string(evaluateBinary(register1));
    register2 = "R" + to_string(evaluateBinary(register2));
    //bneOffset = to_string(evaluateBinary(bneOffset));
		if (bneOffset.substr(0,4) == "1111")
    {
      bneOffset = to_string(dosComplement(bneOffset));
    } else
    {
      bneOffset = to_string(evaluateBinary(bneOffset));
    }
    instruction = instruction + " " + register1 + ", " + register2 + ", " + "#" + bneOffset;
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, stoi(bneOffset), register1, register2, "", "", 1);
  } else if (line.substr(3,3) == "100")
  {
    instruction = "BGTZ";
    string register1 = line.substr(6,5);
    string register2 = "0";
    string bgtzOffset = line.substr(16);
    register1 = "R" + to_string(evaluateBinary(register1));
    register2 = "R" + register2;
    bgtzOffset = to_string(dosComplement(bgtzOffset));
    instruction = instruction + " " + register1 + ", #" + bgtzOffset;
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, stoi(bgtzOffset), register1, "", "", "", 1);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, stoi(offset), register1, base, "", "", 1);
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
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, stoi(offset), register1, base, "", "", 1);
  } else if (line.substr(3,3) == "111")
  {
    instruction = "BREAK";
    broke = true;
		//InstCat(string inst, int tar, string reg1, string reg2, string dest, string immed, int cat)
		MIPSInstructions[address] = InstCat(instruction, 0, "", "", "", "", 1);
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

int execute(InstCat t, int address)
{
	int cat = t.type;
	int newAddress = -1000;
	//int type = t.type;
	string instruction = t.instruction;

	if (cat == 1)
	{
		if (instruction.substr(0,3) == "NOP")
		{
			// nothing happens here
		} else if (instruction.substr(0,1) == "J")
		{
			int target = t.target;
			newAddress = target;
		} else if (instruction.substr(0,3) == "BEQ")
		{
			int register1 = stoi(t.register1.substr(1));
			int register2 = stoi(t.register2.substr(1));
			register1 = registers[register1];
			register2 = registers[register2];
			if (register1 == register2)
			{
				int target = t.target;
				newAddress = address + 4 + (target*4);
			}
		} else if (instruction.substr(0,3) == "BNE")
		{
			int register1 = stoi(t.register1.substr(1));
			int register2 = stoi(t.register2.substr(1));
			register1 = registers[register1];
			register2 = registers[register2];
			if (register1 != register2)
			{
				int target = t.target;
				newAddress = address + 4 + (target*4);
			}
		} else if (instruction.substr(0,4) == "BGTZ")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			if (register1 > 0)
			{
				int target = t.target;
				newAddress = address + 4 + (target*4);
			}
		} else if (instruction.substr(0,2) == "SW")
		{
			// data[base + offset] = register1
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			int base = stoi(t.register2.substr(1));
			base = registers[base];
			int offset = t.target;
			int memory = base + offset;
			memory = (memory - firstData)/4;
			data[memory] = register1;
		} else if (instruction.substr(0,2) == "LW")
		{
			// register 1 = data[base+offset]
			int base = stoi(t.register2.substr(1));
			base = registers[base];
			//cout << "base = " << base << "\n";
			int offset = t.target;
			//cout << "offset = " << offset << "\n";
			int memory = base + offset;
			memory = (memory - firstData)/4;
			//cout << "memory = " << memory << "\n";
			int register1 = stoi(t.register1.substr(1));
			//cout << "register 1 = " << register1 << "\n";
			registers[register1] = data[memory];
		} else if (instruction.substr(0,5) == "BREAK")
		{
			brokenAgain = true;
		}
	} else if (cat == 2)
	{
		if (instruction.substr(0,3) == "XOR")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			int register2 = stoi(t.register2.substr(1));
			binary(register1);
			string binary1 = signExtend();
			ss.str( std::string() );
			ss.clear();
			binary(register2);
			string binary2 = signExtend();
			ss.str( std::string() );
			ss.clear();
			string xorString = XORcompare(binary1, binary2);
			xorString = twoComplement(xorString);
			int value = dosComplement(xorString) + 2;
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,4) == "ADDU")
		{
			int register1 = stoi(t.register1.substr(1));
			int register2 = stoi(t.register2.substr(1));
			int value = registers[register1] + registers[register2];
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,4) == "SUBU")
		{
			int register1 = stoi(t.register1.substr(1));
			int register2 = stoi(t.register2.substr(1));
			int value = registers[register1] - registers[register2];
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,3) == "MUL")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			int register2 = stoi(t.register2.substr(1));
			register2 = registers[register2];
			int value = register1 * register2;
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,3) == "ADD")
		{
			int register1 = stoi(t.register1.substr(1));
			int register2 = stoi(t.register2.substr(1));
			int value = registers[register1] + registers[register2];
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,3) == "SUB")
		{
			int register1 = stoi(t.register1.substr(1));
			int register2 = stoi(t.register2.substr(1));
			int value = registers[register1] - registers[register2];
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,3) == "AND")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			int register2 = stoi(t.register2.substr(1));
			binary(register1);
			string binary1 = signExtend();
			ss.str( std::string() );
			ss.clear();
			binary(register2);
			string binary2 = signExtend();
			ss.str( std::string() );
			ss.clear();
			string andString = ANDcompare(binary1, binary2);
			andString = twoComplement(andString);
			int value = dosComplement(andString) + 2;
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,2) == "OR")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			int register2 = stoi(t.register2.substr(1));
			binary(register1);
			string binary1 = signExtend();
			ss.str( std::string() );
			ss.clear();
			binary(register2);
			string binary2 = signExtend();
			ss.str( std::string() );
			ss.clear();
			string orString = ORcompare(binary1, binary2);
			orString = twoComplement(orString);
			int value = dosComplement(orString) + 2;
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		}
	} else if (cat == 3)
	{
		if (instruction.substr(0,3) == "ORI")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			int immediate = stoi(t.immediate);
			binary(register1);
			string binary1 = signExtend();
 			// clears the string stream ss
			ss.str( std::string() );
			ss.clear();
			binary(immediate);
			string binary2 = signExtend();
			// clears the string stream ss
			ss.str( std::string() );
			ss.clear();
			string ori = ORcompare(binary1, binary2);
			ori = twoComplement(ori);
			int value = dosComplement(ori) + 2;
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,4) == "XORI")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			//cout << "the value in register 1 is equal to " << register1 << "\n";
			int immediate = stoi(t.immediate);
			//cout << "the immediate value is equal to " << immediate << "\n";
			binary(register1);
			string binary1 = signExtend();
			//cout << "the binary representation of register 1: " << binary1 << "\n";
 			// clears the string stream ss
			ss.str( std::string() );
			ss.clear();
			binary(immediate);
			string binary2 = signExtend();
			//cout << "the binary representation of the immediate value: " << binary2 << "\n";
			// clears the string stream ss
			ss.str( std::string() );
			ss.clear();
			string xori = XORcompare(binary1, binary2);
			//cout << "the xor binary value of the two strings: " << xori << "\n";
			xori = twoComplement(xori);
			int value = dosComplement(xori) + 2;
			//cout << "the value of the xori string is: " << value << "\n";
			int destination = stoi(t.destination.substr(1));
			//cout << "registers[" << destination << "] should equal the XORI value = " << value << "\n";
			registers[destination] = value;
		} else if (instruction.substr(0,4) == "ADDI")
		{
			int register1 = stoi(t.register1.substr(1));
			int immediate = stoi(t.immediate);
			int value = registers[register1] + immediate;
			//cout << "THE IMMEDIATE VALUE IS = " << immediate << "\n";
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
			//data[register1]
		} else if (instruction.substr(0,4) == "SUBI")
		{
			int register1 = stoi(t.register1.substr(1));
			int immediate = stoi(t.immediate);
			int value = registers[register1] - immediate;
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,4) == "ANDI")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			int immediate = stoi(t.immediate);
			binary(register1);
			string binary1 = signExtend();
 			// clears the string stream ss
			ss.str( std::string() );
			ss.clear();
			binary(immediate);
			string binary2 = signExtend();
			// clears the string stream ss
			ss.str( std::string() );
			ss.clear();
			string andi = ANDcompare(binary1, binary2);
			andi = twoComplement(andi);
			int value = dosComplement(andi) + 2;
			int destination = stoi(t.destination.substr(1));
			registers[destination] = value;
		} else if (instruction.substr(0,3) == "SRL")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			// immediate is equivalent to the shift amount
			register1 = shiftRight(register1, stoi(t.immediate));
			int destination = stoi(t.destination.substr(1));
			registers[destination] = register1;
		} else if (instruction.substr(0,3) == "SRA")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			// immediate is equivalent to the shift amount
			register1 = shiftRight(register1, stoi(t.immediate));
			int destination = stoi(t.destination.substr(1));
			registers[destination] = register1;
		} else if (instruction.substr(0,3) == "SLL")
		{
			int register1 = stoi(t.register1.substr(1));
			register1 = registers[register1];
			// immediate is equivalent to the shift amount
			register1 = shiftLeft(register1, stoi(t.immediate));
			int destination = stoi(t.destination.substr(1));
			registers[destination] = register1;
		}
	} else
	{
		// blah blah blah
	}

	return newAddress;
}

void printSimulation(ofstream& simulationfile, int cycle, int address, InstCat t)
{
	simulationfile << "--------------------\n" << "Cycle " << cycle << ":\t" << address << "\t"
		<< t.instruction << "\n\n" << "Registers\n" << "R00:\t" << registers[0] << "\t" << registers[1] << "\t"
		<< registers[2] << "\t" << registers[3] << "\t" << registers[4] << "\t" << registers[5]
		<< "\t" << registers[6] << "\t" << registers[7] << "\n" << "R08:\t" << registers[8] << "\t"
		<< registers[9] << "\t" << registers[10] << "\t" << registers[11] << "\t" << registers[12]
		<< "\t" << registers[13] << "\t" << registers[14] << "\t" << registers[15] << "\n"
		<< "R16:\t" << registers[16] << "\t" << registers[17] << "\t" << registers[18] << "\t" << registers[19]
		<< "\t" << registers[20] << "\t" << registers[21] << "\t" << registers[22] << "\t"
		<< registers[23] << "\n" << "R24:\t" << registers[24] << "\t" << registers[25] << "\t"
		<< registers[26] << "\t" << registers[27] << "\t" << registers[28] << "\t" << registers[29]
		<< "\t" << registers[30] << "\t" << registers[31] << "\n\n" << "Data\n" << firstData << ":\t"
		<< data[0] << "\t" << data[1] << "\t" << data[2] << "\t" << data[3] << "\t" << data[4] << "\t"
		<< data[5] << "\t" << data[6] << "\t" << data[7] << "\n" << firstData+32 << ":\t" << data[8]
		<< "\t" << data[9] << "\t" << data[10] << "\t" << data[11] << "\t" << data[12] << "\t"
		<< data[13] << "\t" << data[14] << "\t" << data[15] << "\n\n" ;
}

void simulate(ofstream& simulationfile, int cycle, int address)
{
	InstCat t = MIPSInstructions.at(address);

	int executionAddress = execute(t, address);

	printSimulation(simulationfile, cycle, address, t);
	cycle++;

	if (executionAddress == -1000)
		executionAddress = address+4;

	if (!brokenAgain)
		simulate(simulationfile, cycle, executionAddress);
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
		int dataMember = 0;
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
				// Store the data values in the correct data array
				data[dataMember] = dosValue;
				if (dataMember == 0)
				{
					firstData = address;
				}
				dataMember++;
      }
    }
    disassemblyfile.close();
    inputfile.close();
  } else
  {
    cout << "Unable to open file";
  }

	// Call simulation method to complete the simulation of the MIPS instructions
	// Create the simulation file
	ofstream simulationfile ("simulation.txt");
	simulate(simulationfile, 1, 64);
	simulationfile.close();

  return 0;
}
