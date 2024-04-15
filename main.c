#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>

char *get_executable_name(char *szPath) {

  ssize_t n = readlink("/proc/self/exe", szPath, 300);
  if (n < 0) {
    return NULL;
  }
  szPath[n] = '\0';

}

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *file1 = fopen("bufferrequest.txt", "w");
    if (file1 == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    fwrite(ptr, size, nmemb, file1);  // Écriture des données dans le fichier

    fclose(file1);
    return size * nmemb;
}


int request(char link[]){
  CURL *curl = curl_easy_init();
  CURLcode se;
  if (!curl){
    return 1;
  }
  curl_easy_setopt(curl, CURLOPT_URL, link);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

  se = curl_easy_perform(curl);
  if (se != CURLE_OK){
    return 2;
  }
  

  curl_easy_cleanup(curl);
  return 0;
  
}

int main(void){
  char path[1024];
  ssize_t size = readlink("/proc/self/exe", path, sizeof(path)-1);
  if (size != -1){
    printf("chemin absolu vers l'executable : %s\n", path);
  }
  else {
    printf("error : path find : %s\n", path);
  }

  char path2[1024];
  if (getcwd(path2, sizeof(path2)-1) != NULL){
    printf("chemin absolu vers le dossier : %s", path2);
    
  }
  else {
    printf("error voici le chemin obtenu : %s", path2);
  }

  FILE *file1;
  file1 = fopen(path, "rb");
  if (file1 == NULL) {
      perror("Error opening file1");
      return 1;
  }
  char content[2000];
  char lines[400];
  while (fgets(lines, sizeof(lines), file1) != NULL) {
    strcat(lines, content);
}
  
  printf("\n%s\n", content);

  char a[] = "https://www.youtube.com/watch?v=eXebqmlXmac";

  char name[300];
  get_executable_name(name);
  char buffer[strlen(path2)];
  strcpy(buffer, "");
  strcat(buffer, path2);
  strcat(buffer, "/");
  strcat(buffer, name);
  fclose(file1);
  

  if (strcmp(buffer, path) == 0){
    remove(path);
  }
  else{
    buffer[0] = '\0';
    strcat(buffer, path2);
    strcat(buffer, "/");
    strcat(buffer, name);
    FILE *file2 = fopen(buffer, "wb");
    fputs(content, file1);
    fclose(file2);
    
  }

  printf("bonjour");

  int req_result = request("https://www.google.com");
  
  if (req_result != 0) {
        fprintf(stderr, "La requête a échoué avec le code d'erreur %d\n", req_result);

        return 1;
    }

  strcpy(content, " ");

  file1 = fopen("bufferrequest.txt", "r");
  while (fgets(lines, sizeof(lines), file1) != NULL){
    strcat(content, lines);
  }
  printf("%s", content);

  return 0;
}
