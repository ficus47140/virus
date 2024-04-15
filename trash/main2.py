import psutil
import time

def afficher_utilisation():
  """Affiche l'utilisation de la RAM et du CPU."""
  # Récupération des données d'utilisation
  utilisation_cpu = psutil.cpu_percent()
  utilisation_memoire = psutil.virtual_memory().percent

  # Formatage des données
  utilisation_cpu_str = f"CPU: {utilisation_cpu}%"
  utilisation_memoire_str = f"RAM: {utilisation_memoire}%"

  # Affichage des données
  print(f"{utilisation_cpu_str} | {utilisation_memoire_str}")

# Boucle principale
while True:
  afficher_utilisation()
  time.sleep(2)  # Pause de 2 secondes
