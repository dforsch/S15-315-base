volatile int distanceToTravelLeft;
volatile int distanceToTravelRight;


void setDistanceToTravelRight(int inches){
	distanceToTravelRight = 75.143*inches;
}

void setDistanceToTravelLeft(int inches){
	distanceToTravelLeft = 75.143*inches;
}