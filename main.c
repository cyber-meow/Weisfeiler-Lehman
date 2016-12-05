#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>

#include "weisfeiler-lehman.h"

void err(char *msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(1);
}


int main(int argc, char *argv[])
{
  int action, c;
  FILE *ifile;

  /* parsing options */
  struct option longopts[] = {
    { "naif", no_argument, &action, 'n'},
    { "degree", no_argument, &action, 'd'},
    { "check", no_argument, &action, 'c'},
    { "partition", no_argument, &action, 'P'},
    { "permu", no_argument, &action, 'p'},
    { "graph", no_argument, &action, 'g'},
    { "rgraph", no_argument, &action, 'r'},
    { "transform", no_argument, &action, 't'},
    { "help", no_argument, &action, 'h'},
    { 0, 0, 0, 0 }
  };

  while((c = getopt_long(argc, argv, "", longopts, NULL)) != -1)
  {
    if(c == '?')
      err("use --help for more information");
  }
   
  /* utilisé quand il y a un fichier en entrée*/
  void check_input_file(int arg_num)
  {
    if(argc < arg_num)
      err("Input file name missing.");
    if(argc > arg_num)
      err("Only one input file can be supplied.");
    
    ifile = fopen(argv[arg_num-1], "r");
    if(ifile == 0)
      err("Input file cannot be found.");
  }

  if(optind > 2)
    err("At most one option can be supplied.");

  else if(optind == 1)
  {
    check_input_file(2);

    /* lire les deux graphs */
    graph *G1 = get_graph(ifile);
    graph *G2 = get_graph(ifile);
    array *iso = isomorphism(G1, G2);
    print_permutation(iso);
    
    if(iso)
      array_destroy(iso);
    graph_destroy(G1);
    graph_destroy(G2);

    fclose(ifile);
  }

  else
  {
    switch(action)
    {
      case 'n':

        check_input_file(3);
        graph *G1 = get_graph(ifile);
        graph *G2 = get_graph(ifile);
        array *iso = isomorphism_naif(G1, G2);
        print_permutation(iso);
    
        if(iso)
          array_destroy(iso);
        graph_destroy(G1);
        graph_destroy(G2);
        fclose(ifile);
        break;

      case 'd':

        check_input_file(3);
        G1 = get_graph(ifile);
        G2 = get_graph(ifile);
        iso = isomorphism_degree(G1, G2);
        print_permutation(iso);
    
        if(iso)
          array_destroy(iso);
        graph_destroy(G1);
        graph_destroy(G2);
        fclose(ifile);
        break;

      case 'c':

        check_input_file(3);
        G1 = get_graph(ifile);
        G2 = get_graph(ifile);
        iso = get_array(ifile);

        if(verify_isomorphism(G1, G2, iso))
          printf
            ("These two graphs are isomorphic via the given permutation\n");
        else
          printf
            ("These two graphs aren't isomorphic via the given permutation\n");
  
        array_destroy(iso);
        graph_destroy(G1);
        graph_destroy(G2);
        fclose(ifile);
        break;

      case 'P':

        check_input_file(3);
        G1 = get_graph(ifile);
        partition *P = get_partition(ifile);
        
        partition *P_ = stable_partition(G1, P);
        partition_print(P_);

        graph_destroy(G1);
        partition_destroy(P);
        partition_destroy(P_);
        break;

      case 'p':
        
        for(int i=optind;i<argc;++i)
        {
          int n = strtol(argv[i], NULL, 10);
          array *pho = random_permutation(n);
          array_print_simple(pho);
          array_destroy(pho);
        }
        break;

      case 'g':

        if(argc != 4)
          err("Wrong number of input arguments.");
        
        int n = strtol(argv[2], NULL, 10);
        float p = strtof(argv[3], NULL);
        G1 = random_graph(n,p);
        graph_print_simple(G1);
        graph_destroy(G1);
        break;

      case 'r':

        if(argc != 4)
          err("Wrong number of input arguments.");
        
        n = strtol(argv[2], NULL, 10);
        int k = strtol(argv[3], NULL, 10);
        G1 = generate_regular_graph(n,k,500);
        graph_print_simple(G1);
        graph_destroy(G1);
        break;
        
      case 't':

        check_input_file(3);
        G1 = get_graph(ifile);
        iso = get_array(ifile);
        G2 = graph_transform(G1, iso);
        graph_print_simple(G2);

        graph_destroy(G1);
        graph_destroy(G2);
        array_destroy(iso);
        break;

      case 'h':

        puts("The program checks if two given graphs are isomorphic");
        puts("Usage: main [option] <inputs>");
        puts("OPTIONS:");
        puts("  --naif        Run the naif isomorphic algorithm instead");
        puts("  --degree      Run the isomorphic algorithm based on degree partitions instead");
        puts("  --partition   Stable partition algorithm");
        puts("  --check       Check if two graphs are isomorphic via given bijection");
        puts("  --transform   Apply a bijection on a graph");
        puts("  --permu       Generate a random permutation");
        puts("  --graph       Generate a random graph");
        puts("  --rgraph      Generate a random regular graph");
        puts("  --help        Display this list of information");
        puts("See LISEZMOI for more details");
        break;
    }
  }


  return 0;
}
