//Bryce Randolph
//Lab05
//2/25/18
//This program takes an input maze image bmp and uses breadth first search to draw a line from the start to the end.
//It then outputs that solved maze image into a bmp
#include<iostream>
#include<cstdlib>
#include<vector>
#include<queue>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include"EasyBMP.h"

using namespace std;
//this function takes a bmp as input and converts it to a black and white image
void colorToGray(BMP & input)
{
  int picWidth = input.TellWidth();
  int picHeight = input.TellHeight();
  input.SetBitDepth(1); //compression happens here
  for (int i = 0; i < picWidth-1; ++i)
    for (int j = 0; j < picHeight-1; ++j)
      {
	int col = 0.1* input(i, j)->Blue +
	  0.6*input(i,j)->Green + 0.3* input(i,j)->Red;
	if (col > 127) {
	  input(i,j)->Red = 255;
	  input(i,j)->Blue = 255;
	  input(i,j)->Green = 255;
	}
      }
}
//this breadth first search algorithm takes a bmp input, as well as two ints a starting and ending point, and an int count variable
//This determines and draws the shortest line from start to end
void BFS(BMP & input, int start, int end, int count)
{
  queue<int> Q;
  Q.push(start);
  count++;
  
  int width = input.TellWidth();
  int height = input.TellHeight();
  
  int numPixels = input.TellHeight() * input.TellWidth();
  
  bool* visited = new bool[numPixels];
  int v;
  int * distance = new int[numPixels];
  
  for(int i = 0; i < numPixels; i++)
    {
      visited[i] = false;
    }
  visited[start] = true;
  distance[start] = 0;
  
  int * parent = new int[numPixels];
  for(int i = 0; i < input.TellHeight(); i++)//make sure to set all black pixels to visited#################
    for(int j = 0; j < input.TellWidth(); j++)
      {
	int TotalColor = input(j,i)->Red + input(j,i)->Blue + input(j,i)->Green;
	if(TotalColor == 0)
	  {
	    int q = (input.TellWidth() *i) + j;
	    visited[q] = true;
	  }
      }
  while((Q.size() > 0) && visited[end] == false)
    {
      int u = Q.front();
      int column = u % input.TellWidth();
      int row = u / input.TellWidth();
      
      int right = u + 1;
      int left = u - 1;
      int top = u - input.TellWidth();
      int bottom = u + input.TellWidth();
      Q.pop();
      if(row > 0 and !visited[top])
	{
	  visited[top] = true;
	  distance[top] = distance[u] + 1;
	  parent[top] = u;
	  Q.push(top);
	  count++;
	}
      if(row < (input.TellHeight() - 1) and !visited[bottom])
	{
	  visited[bottom] = true;
	  distance[bottom] = distance[u] + 1;
	  parent[bottom] = u;
	  Q.push(bottom);
	  count++;
	}
      if(column > 0 and !visited[left])
	{
	  visited[left] = true;
	  distance[left] = distance[u] + 1;
	  parent[left] = u;
	  Q.push(left);
	  count++;
	}
      if(column < (input.TellWidth() - 1) and !visited[right])
	{
	  visited[right] = true;
	  distance[right] = distance[u] + 1;
	  parent[right] = u;
	  Q.push(right);
	  count++;
	}
    } //end while
  if(visited[end] == true)
    {
      int v = end;
      while(v != start)
	{
	  int X = v % input.TellWidth();
	  int Y = v / input.TellWidth();
	  input(X,Y)->Red = 255;
	  input(X,Y)->Green = 0;
	  input(X,Y)->Blue = 0; 
	  
	  v = parent[v];
	}
      cout << "Shortest Path: "<< distance[end] << " pixels\n";
      cout << "Total Queue Size: "<< count << " pixels\n";
      
    }
  else
    {
      cout << "No path from start to end!";      
    }
}
//main that takes two additional arguments, input filename and output filename
int main(int argc, char * argv[])
{
  if(argc !=3)
    {
      cout << "Usage: ./a.out <input_filename> <output_filename>" << endl;
      exit(1);
    }
  BMP input;
  input.ReadFromFile(argv[1]);
  colorToGray(input);
  input.SetBitDepth(24);
  int sx;int sy;
  int ex;int ey; 
  cout<<"The input image size is "<<input.TellWidth()<<" x "<<input.TellHeight()<<endl;  
  
  cout<<"Enter the x and y coordinates of the starting point on separate lines:\n";
  cin>>sx;
  cin>>sy;
  cout<<"Enter the x and y coordinates of the ending point on separate lines:\n";
  cin>>ex;
  cin>>ey;
  
  //checking if entered coordinated are acceptable
  if(sx < 0 || sy < 0 || ex < 0 || ey < 0 || sx > input.TellWidth() || sy > input.TellHeight() || ex > input.TellWidth() || ey > input.TellHeight())
    {
      cout<<"You entered coordinates outside of the input image!\n";
      exit(1);
    }
  //checking to make sure start and end are not black
  int startColor = input(sx,sy)->Red + input(sx,sy)->Blue + input(sx,sy)->Green;
  int endColor  = input(ex,ey)->Red + input(ex,ey)->Blue + input(ex,ey)->Green;
  if(startColor!= 765)
    {
      cout<<"You entered a starting pixel that was black!\n";
      exit(1);
    }
  if(endColor!= 765)
    {
      cout<<"You entered an ending pixel that was black!\n";
      exit(1);
    }
  int widthMid = input.TellWidth()/2;
  int heightMid = input.TellHeight()/2;
  
  int start = sx + input.TellWidth() * sy;
  int end = ex + input.TellWidth() * ey;
  int count = 0;
  
  BFS(input, start, end, count);
  double finish_time = 0;
  double start_time = clock();
  input.WriteToFile(argv[2]);
  finish_time = clock();
  double time = (double) (finish_time - start_time)/ CLOCKS_PER_SEC;
  cout<<"Time: "<<time<<endl;
  
 return 0;
}
