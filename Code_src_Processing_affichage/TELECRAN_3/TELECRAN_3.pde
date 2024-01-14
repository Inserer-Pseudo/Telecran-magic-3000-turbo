import processing.serial.*;

Serial myPort;
int lf = 10;    // Linefeed in ASCII
int[] coords = new int[2];

PImage FondEcran;

void setup() {
  FondEcran = loadImage("fondEcran.jpg");
  
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.clear();
  
  size(800, 600); // Définir la taille de la fenêtre
  clearScreen(); // Effacer l'écran
}

void draw() {
  if (myPort != null && myPort.available() > 0){
    String data = myPort.readStringUntil(lf);
    if (data != null) {
      data = data.trim(); // Supprime les espaces en début et fin de chaîne

      if ("clear".equals(data)) {
          clearScreen();
      }
      else if (!data.isEmpty() && !"clearOK".equals(data)) {
          int[] newCoords = parser(data);
          if (newCoords != null) {
            coords = newCoords;
          }
      }
      
      if (coords != null) {
        int CouleurPoint = color(map(coords[0], 0, width, 0, 255), map(coords[1], 0, height, 0, 255), map(coords[0] + coords[1], 0, width + height, 50, 200)); // permet l'affichage de point de couleur différentes en fonction de la position X/Y (echelle sur RGB avec bleu borné pour par avoir des points blanc
        fill(CouleurPoint);
        ellipse(coords[0], coords[1], 10, 10); // Dessiner un cercle aux coordonnées x et y
      }
    }
  }
}

void clearScreen() {
  background(255); // Définir la couleur de fond à blanc
  background(FondEcran);
  println("clearOK");
}

int[] parser(String string){
  String[] parts = split(string, ',');
  int[] coords = new int[2];
  coords[0] = Integer.parseInt(parts[0]);
  coords[1] = Integer.parseInt(parts[1]);
  
  return coords;
}
