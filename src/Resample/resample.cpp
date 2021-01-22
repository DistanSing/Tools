/*
 *		Copyright(c) 2021 - Jacob Blomquist
 *
 *		Permission is hereby granted, free of charge, to any person obtaining a copy
 *		of this softwareand associated documentation files(the "Software"), to deal
 *		in the Software without restriction, including without limitation the rights
 *		to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 *		copies of the Software, and to permit persons to whom the Software is
 *		furnished to do so, subject to the following conditions :
 *
 *		The above copyright noticeand this permission notice shall be included in all
 *		copies or substantial portions of the Software.
 *
 *		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *		SOFTWARE.
 */

#include <fstream>
#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv)
{

	if (argc != 4)
	{
		printf("USAGE: %s <input file> <input sample rate> <output sample rate>\n\n The Input file should be a mono, 16bit (short),little-endian pcm file.\n\n Outputs to \"output.pcm\"", argv[0]);
		return 1;
	}


	std::ifstream input;
	input.open(argv[1], std::ifstream::ate | std::ifstream::binary);

	if (!input.is_open())
	{
		printf("Unable to open file!\n");
		return 1;
	}


	int originalRate = atoi(argv[2]);//44100;

	if (!originalRate)
	{
		printf("Invalid Input Sample Rate: %s\n", argv[2]);
		return 1;
	}

	int targetRate = atoi(argv[3]); //10000;

	if (!targetRate)
	{
		printf("Invalid Output Sample Rate: %s\n", argv[3]);
		return 1;
	}


	uint32_t fileSize = input.tellg();
	input.seekg(0);

	int numChannels = 1;

	int numSamples = fileSize / numChannels / sizeof(short);

	printf("Num Samples: %d\n", numSamples);




	double sampleRatio = (double)originalRate / (double)targetRate;
	printf("Ratio: %lf\n", sampleRatio);


	uint32_t destSamples = (uint32_t)((double)numSamples / (double)originalRate * (double)targetRate);
	if (destSamples % 2)
		destSamples++;

	short* inputData = new short[numSamples];
	input.read((char*)inputData, fileSize);


	short* destination = new short[destSamples];

	printf("Target Samples: %d\n", destSamples);

	double sourceIter = 0;


	for (int i = 0; i < destSamples; i++)
	{
		if (sourceIter >= numSamples) {
			break;
		}

		destination[i] = inputData[(int)sourceIter];
		sourceIter += sampleRatio;
	}
	input.close();

	std::ofstream output;
	output.open("output.pcm", std::ofstream::binary);
	output.write((const char*)destination, destSamples * 2);

	output.close();
	delete[] inputData;
	delete[] destination;

	return 0;
}