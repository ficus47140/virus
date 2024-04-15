#include <stdio.h>
#include <xmrig/xmrig.h>

int main() {
  // Remplacez "wallet_address" par votre adresse de portefeuille Monero
  xmrig_set_address("LSBenacd7Vxq5ovfpZbXMFR3CkvsheFVrb", 0);

  // Définissez le nombre de threads à utiliser (adaptez en fonction de votre CPU)
  xmrig_set_threads(4);

  // Démarrez le minage
  xmrig_start();

  // Attendez indéfiniment
  while (1) {
    sleep(1);
  }

  return 0;
}
