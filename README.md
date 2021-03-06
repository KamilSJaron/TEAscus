# TEAscus

Transposable element simulator of haploid yeast with sexual reproduction though ascus.

This simulator is intended as a submodule of a different repository (yeast_TE_load_prediction) and the installation is designed as such. Follow please the following steps to get it running.

### Installation

```
git clone --recursive https://github.com/KamilSJaron/yeast_TE_load_prediction
cd yeast_TE_load_prediction/TEAscus
make
# make test
make install [INSTAL_PREFIX=/where/to/install]
```

default installation location is `INSTAL_PREFIX=/usr/local` and `make` always add `/bin` in the end of the path.

### Usage

most of parameters are read form an input file `input.txt`.

```
Usage:
         TEAscus [options]
Options:
         [-h]            print help of TEAscus and die
         [-v]            print version of TEAscus and die
         [-s]            sex every [-p] generations (False)
         [-g]    INT     number of generations (990)
         [-b]    INT     number of burn-in generations before simumlation (20)
         [-r]    INT     number of replicates (10)
         [-p]    INT     period of saves and sex if -s option is set (90)
         [-i]    FILE    name of input file (input.txt)
         [-o]    FILE    name of output file (detailed.txt)
```

Few parameters are still hardcoded in`src/Genome.cpp` : number of chromosomes, number of loci, recombination rates.

The rest of parameters are loaded from file `input.txt`

```
Line 1: N (population size)
Line 2: mod_init (initial frequency of the modifier in the population)
Line 3: u_mitosis (transposition rate during mitosis u - asexual)
Line 4: u_meiosis (transposition rate during meiosis u - sexual)
Line 5: v_base (non modified excision rate v)
Line 6: v_modified (exision rate boosted by modifier)
Line 7: sa (a, selection parameter)
Line 8: sb (b, synergism within family selection parameter)
Line 9: n_0 (number of TEs at the beginning)
```

### Details of simulation

Simulator is created to reproduce experiment of [McDonald et al.](www.nature.com/doifinder/10.1038/nature17143) as closely as possible. Simulations run for *990 generations*, every 90 generations there is (or isn't) a sexual generation, this is also the generation when stats are captured (after reproduction).

 * 16 segregating Chromosomes
 * 500 slots (loci) for TEs on each chromosome (i.e. 8000 in total)
 * recombination rates of each chromosome are modeled as poisson process with 5.6 chiasmata according to [mapping of meiotic crossovers in yeast](dx.doi.org/10.1038/nature07135) totaling in ~90 crossovers per meiosis.

Asexual reproduction is clonal and perfect (no mutation). Sexual reproduction is reformed through fusion of two haploid individuals into **ascus** followed by meiosis, producing four new haploid individuals (hence the name of the simulator).

The selection is acting on haploid individuals, not on diploid asci. The population is of fixed size and the choice of individuals to next generation is implemented using Monte Carlo (just like in the original simulator).

### Testing

Unit tests are written using [CppUnit framework](https://wiki.freedesktop.org/www/Software/cppunit/) saved in `test/` and their headers are in `include`. Every class has one test class and the naming scheme is straightforward (just prefix `Test` before a class name).

To run test write command

```
make test
```

which automatically (re)compiles what is needed, compile testing program, run it and if it passes then also delete the testing program.

The tests were written and tested on OS X using `clang` compiler. Simple test on Ubuntu with CppUnit installed though `apt-get` have failed with non-trivial error. Pull-requests are welcomed on this issue.

### Origin of the code

The original code was provided by Elie Dolgin and it was used for publication "[The Fate of Transposable Elements in Asexual Populations](https://doi.org/10.1534/genetics.106.060434)"; DOI: 10.1534/genetics.106.060434
(the code as it was shared with me is accessible in commit [7daac13](https://github.com/KamilSJaron/yeast_TE_load_prediction/tree/7daac13fdccd4b5ce1d2c7f169c63a269f9442cc/Transposon)). The software was called simply Transposon and it have implemented asexual reproduction and sexual reproduction of diploid individuals.

The main modification are enforced haploid individuals with possible asexual and sexual reproduction modes (sex through ascus).

- I added unit tests
- simplified code on several places (deleting getters of static variables etc)
- Logging was moved to error stream from standard stream, which was cleared for printing output if ever desired.
- When development code is compiled, git commit is saved in binary and it's printed to log every time simulator is executed
- hardcoded parameters are turned at least to define directives whenever possible
