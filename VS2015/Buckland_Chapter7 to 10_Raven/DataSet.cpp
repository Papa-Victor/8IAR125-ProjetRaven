#include "DataSet.h"

using namespace std;

constexpr char Separator = ',';

ostream& operator<<(ostream& os, const Input& input)
{
	if (sizeof(input.values))
	{
		os << input.values[0];

		for (int i = 1; i < sizeof(input.values) / sizeof(input.values[0]); ++i)
		{
			os << Separator << input.values[i];
		}
	}

	return os;
}

istream& operator>>(istream& is, Input& input)
{
	if (sizeof(input.values))
	{
		char separator;
		is >> input.values[0];

		for (int i = 1; i < sizeof(input.values) / sizeof(input.values[0]); ++i)
		{
			is >> separator >> input.values[i];
			if (separator != Separator)
			{
				throw runtime_error{ "The parser expects comma separated floats." };
			}
		}
	}
	
	return is;
}

ostream& operator<<(ostream& os, const Output& output)
{
	os << output._shouldFire;
	return os;
}

istream& operator>>(istream& is, Output& output)
{
	is >> output._shouldFire;
	return is;
}

void DataSet::Add(const Input& input, const Output& output)
{
	this->_dirty = true;
	this->_inputs.push_back(input);
	this->_outputs.push_back(output);
}

const vector<float>& DataSet::Inputs()
{
	if (this->_dirty)
	{
		for (const auto& input : this->_inputs)
		{
			this->_floatInputs.insert(this->_floatInputs.end(), &input.values[0], &input.values[0] + sizeof(input.values) / sizeof(input.values[0]));
		}
	}

	return this->_floatInputs;
}

const vector<float>& DataSet::Outputs()
{
	if (this->_dirty)
	{
		for (const auto& output : this->_outputs)
		{
			this->_floatOutputs.push_back(output._shouldFire);
		}
	}

	return this->_floatOutputs;
}

ostream& operator<<(ostream& os, DataSet& dataSet)
{
	if (dataSet._inputs.size())
	{
		for (int i = 0; i < dataSet._inputs.size(); ++i)
		{
			os << dataSet._inputs[i] << Separator << dataSet._outputs[i] << endl;
		}
	}

	return os;
}

istream& operator>>(istream& is, DataSet& dataSet)
{
	char separator;
	while (!is.eof())
	{
		Input input;
		Output output;

		is >> input >> separator >> output;
		if (separator != Separator)
		{
			throw runtime_error{ "The parser expects comma separated inputs and outputs." };
		}

		dataSet.Add(input, output);
		is.ignore(std::numeric_limits<int>::max(), is.widen('\n'));
	}

	return is;
}