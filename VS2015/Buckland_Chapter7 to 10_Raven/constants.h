#ifndef CONSTANTS_H
#define CONSTANTS_H

constexpr int WindowWidth  = 500;
constexpr int WindowHeight = 500;
constexpr int FrameRate    = 60;

constexpr char* TrainingFileName = "TrainingData.csv";
constexpr char* PerceptronFileName = "Perceptron";

constexpr float MinimumSuccessRate = 0.95;
constexpr int IterationsPerLog = 100;
constexpr int MaxIterations = 1000;

constexpr unsigned char FramesPerRecord = 15;
constexpr int EntriesPerWrite = 4096;

#endif