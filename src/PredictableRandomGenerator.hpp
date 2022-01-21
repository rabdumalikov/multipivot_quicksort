#pragma once

class RandomNumberStream
{
private:
  unsigned long long u,v,w;

public:
  RandomNumberStream(int n=1);
  double RandomDouble();
  double RandomDouble(double a, double b);
  unsigned long long RandomInteger();
  unsigned long long RandomInteger(int a, int b);
private:
  unsigned long long int64();
};
