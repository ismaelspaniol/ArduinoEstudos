String buscastring(int inicio, int fim, String texto) {
  String valor;
  for (int i = inicio; i <= fim; i++) {
    valor.concat(readString[i]);
  }
  return valor;
}