
#include "e-dijkstra.h"
#include <vector>
#include <deque>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>

typedef std::vector<std::vector<int>> Graph;

struct EV
{


  int numRecharges;
  int range;
};

Graph initGraph(std::fstream &input)
{
  unsigned size;
  int recharges;
  unsigned lineCount;

  input >> size;
  input >> recharges;
  input >> lineCount;

  Graph town(size);

  for(std::vector<int>& columns : town)
  {
    columns.resize(size);
  }

  for(unsigned i = 0; i < lineCount; ++i)
  {
    unsigned from;
    unsigned to;
    int weight;

    input >> from;
    input >> to;
    input >> weight;

    town[from][to] = weight;
    town[to][from] = weight;
  }

  return town;
}


int minDistance(std::vector<EV> dist, std::vector<bool> sptSet) 
{ 
    EV min = { 0, -1 };
    int min_index = 0; 
  
    for (unsigned v = 0; v < dist.size(); v++) 
    {
      

      if (!sptSet[v] && (dist[v].numRecharges >= min.numRecharges) && (dist[v].range > min.range)) 
      {
        min = dist[v];
        min_index = v; 
        
      }
    }
    return min_index; 
} 

EV travel(EV currentRange, int distance, int maxRange)
{
  if((currentRange.range - distance) < 0)
  {
    currentRange.range = maxRange;
    --currentRange.numRecharges;
  }
  currentRange.range -= distance;

  return currentRange;
}

bool e_dijkstra(char const *filename, int range)
{
  std::fstream input(filename);
  Graph town = initGraph(input);
  int maxRecharges;


  input.seekg(0);
  input.ignore(std::numeric_limits<int>::max(), ' ');
  input >> maxRecharges;


  std::vector<EV> remainingCharge(town.size()); 
                                               
  std::vector<int> prev(town.size());
  
  std::vector<bool> sptSet(town.size()); 
  
  
    
  for(unsigned source = 0; source < town.size(); ++source)
  {
    
    for (unsigned i = 0; i < town.size(); i++)
    {
      remainingCharge[i].numRecharges = 0;
      remainingCharge[i].range = -1;
      sptSet[i] = false; 
    } 
   
    remainingCharge[source].numRecharges = maxRecharges - 1;
    remainingCharge[source].range = range;
  
    // Find shortest path for all vertices 
    for (unsigned count = 0; count < town.size() - 1; count++) 
    { 
 
      int u = minDistance(remainingCharge, sptSet); 
      sptSet[u] = true; 
  
      for (unsigned v = 0; v < town.size(); v++) 
      {
    
        if(!sptSet[v] && town[u][v])
        {
          EV newDistance = travel(remainingCharge[u], town[u][v], range);

          
          if((newDistance.numRecharges >= remainingCharge[v].numRecharges) && (newDistance.range > remainingCharge[v].range))
          {
            remainingCharge[v] = newDistance;
            prev[v] = u;
          }
        }
      }
    } 

 

    for(const EV& index : remainingCharge)
    {
      if(index.range == -1)
      {
        return false;
      }
    }
  }
  return true;
}

