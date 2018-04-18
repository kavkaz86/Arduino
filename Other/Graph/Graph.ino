int xPos = 0;       // Horizontal coordinate used to draw the next data point.
int yMin = 0;       // Minimum expected data value.
int yMax = 1023;    // Maximum expected data value.
color c;            // Stroke color used to draw the graph.

// Sets the stroke color used to draw the graph.
void setColor( int r, int g, int b, int a ) {
  c = color( r, g, b, a );
}

void setup() {
  size( 300, 200 );
  frameRate( 50 );
  setColor( 255, 0, 0, 100 );
  drawGrid();
}

void draw() {}    // Empty draw() function.

void drawGrid() {
  int h = height;
  int w = width;
 
  background( 255 );
 
  stroke( 127, 127, 127, 127 );
 
  // Draw horizontal lines.
  line( 0, h / 4, w, h / 4 );
  line( 0, h / 2, w, h / 2 );
  line( 0, h * 3 / 4, w, h * 3 / 4 );
 
  // Draw vertical lines.
  line( w / 4, 0, w / 4, h );
  line( w / 2, 0, w / 2, h );
  line( w * 3 / 4, 0, w * 3 / 4, h );
 
  // Draw labels.
  fill( 0 );
  text( str( yMin ), 5, h - 5 );
  text( str( yMax ), 5, 12 );
}

void update( float data ) {
  // When we reach the edge of the screen, wrap around to the beginning.
  if( xPos >= width ) {
    xPos = 0;
    drawGrid();
  }
 
  // Graph the data point and increment the horizontal coordinate.
  data = map( data, yMin, yMax, 0, height );
  stroke( c );
  line( xPos, height, xPos, height - data );
  xPos++;
}int xPos = 0;       // Horizontal coordinate used to draw the next data point.
int yMin = 0;       // Minimum expected data value.
int yMax = 1023;    // Maximum expected data value.
color c;            // Stroke color used to draw the graph.

// Sets the stroke color used to draw the graph.
void setColor( int r, int g, int b, int a ) {
  c = color( r, g, b, a );
}

void setup() {
  size( 300, 200 );
  frameRate( 50 );
  setColor( 255, 0, 0, 100 );
  drawGrid();
}

void draw() {}    // Empty draw() function.

void drawGrid() {
  int h = height;
  int w = width;
 
  background( 255 );
 
  stroke( 127, 127, 127, 127 );
 
  // Draw horizontal lines.
  line( 0, h / 4, w, h / 4 );
  line( 0, h / 2, w, h / 2 );
  line( 0, h * 3 / 4, w, h * 3 / 4 );
 
  // Draw vertical lines.
  line( w / 4, 0, w / 4, h );
  line( w / 2, 0, w / 2, h );
  line( w * 3 / 4, 0, w * 3 / 4, h );
 
  // Draw labels.
  fill( 0 );
  text( str( yMin ), 5, h - 5 );
  text( str( yMax ), 5, 12 );
}

void update( float data ) {
  // When we reach the edge of the screen, wrap around to the beginning.
  if( xPos >= width ) {
    xPos = 0;
    drawGrid();
  }
 
  // Graph the data point and increment the horizontal coordinate.
  data = map( data, yMin, yMax, 0, height );
  stroke( c );
  line( xPos, height, xPos, height - data );
  xPos++;
}
