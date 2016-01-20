#include <stdio.h>
#include <stdlib.h>

int main() {
  int power, def, terrAtck, terrDef;
  
  printf("Enter power: ");
  scanf("%d", &power);
  
  printf("Enter defense: ");
  scanf("%d", &def);
  
  printf("Enter Attacker Terrain: ");
  scanf("%d", &terrAtck);
  
  printf("Enter Defender Terrain: ");
  scanf("%d", &terrDef);
  
  int damage = (power - def) + (terrAtck - terrDef);
  
  srand(time(NULL));
  
  int diceRoll = rand() % 100;
  
  if (diceRoll > 60 && diceRoll <= 80) {
    damage += 1;
    printf("\nDamage +1\n");
  } else if (diceRoll > 80) {
    damage -= 1;
    printf("\nDamage -1\n");
  } else {
    printf("\nNormal Damage\n");
  }
  
  diceRoll = rand() % 100;
  
  if (diceRoll < 5) {
    damage *= 2;
    printf("Damage x2\n");
  }
  
  if (power - def < 1) {
    diceRoll = rand() % 100;
    if (diceRoll < 25) {
      damage = 1;
      printf("Damage reduced to 1\n");
    }
  }
  
  if (damage < 0)
    damage = 0;
  
  printf("\nDamage to inflict = %d\n", damage);
  
  return 0;
}