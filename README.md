## Plot Elliptic Curve by gnuplot.

![EC plot](/images/ec_screen.png "Screen of plotting elliptic curves")

### This program requires gnuplot to be installed.
It opens gnuplot and sends command to plot EC whenever user orders to do so.

To install gnuplot in Linux:
```bash
apt-get install gnuplot
```

### Another requirements.
Program includes programming library called ncurses which provides terminal API, to install it type e.g.:
```bash
apt-get install libncurses5-dev
```

## Usage
Presents equation y^2 = x^3 + a x + b where one can change parameters:
```
EC: y^2 = x^3 - 43*x + 114
```
Presents x value(s) for point(s) where y = 0 (also on the head of graph):
```
  x for y = 0: -7.613966
```
Computes the value of discriminant parameter for Elliptic Curves:
```
  4*a^3 + 27*b^2 = 32864.0  (-43, 114)
```
> When > 0 then there is only one point for y = 0 and function can be used for Elliptic Curve Cryptography.
>
> When = 0 then there are 2 points for y = 0
>
> When < 0 then it has 3 points for y = 0 and graph has 2 separated parts like on the image below:

![EC graph (two parts)](/images/ec_graph2.png "EC graph (two parts)")


Press enter to plot the equation:
```
 Plot EC
```
Press  enter  to exit
```
 Exit
```

## TODOs
Changing number of points and their precision.
Stepping on points to e.g. show their coordinates and plot the tangent line (optional).

