CFLAGS=-Wall -O3 -std=c++11 -m64
LFLAGS= -lboost_serialization -m64

all: threadtrain threadtest

threadtrain: threadtrain.o perceptron.o segmentor.o featurizer.o util.o naivebayes.o
	clang++ $(LFLAGS) segmentor.o perceptron.o featurizer.o util.o naivebayes.o threadtrain.o -o threadtrain

threadtrain.o: threadtrain.cpp
	clang++ $(CFLAGS) threadtrain.cpp -c

threadtest: threadtest.o perceptron.o segmentor.o featurizer.o util.o naivebayes.o
	clang++ $(LFLAGS) segmentor.o perceptron.o featurizer.o util.o naivebayes.o threadtest.o -o threadtest

threadtest.o: threadtest.cpp
	clang++ $(CFLAGS) threadtest.cpp -c

test: test.o perceptron.o segmentor.o featurizer.o naivebayes.o util.o
	clang++ $(LFLAGS) segmentor.o perceptron.o featurizer.o naivebayes.o util.o test.o -o test

train: train.o perceptron.o segmentor.o featurizer.o util.o naivebayes.o
	clang++ $(LFLAGS) segmentor.o perceptron.o featurizer.o util.o naivebayes.o train.o -o train

train.o: train.cpp
	clang++ $(CFLAGS) train.cpp -c

train_ap: train_ap.o perceptron.o segmentor.o featurizer.o util.o
	clang++ $(LFLAGS) segmentor.o perceptron.o featurizer.o util.o train_ap.o -o train_ap

train_ap.o: train_ap.cpp
	clang++ $(CFLAGS) train_ap.cpp -c

train_nb: train_nb.o naivebayes.o util.o
	clang++ $(LFLAGS) naivebayes.o train_nb.o util.o -o train_nb

train_nb.o: train_nb.cpp
	clang++ $(CFLAGS) train_nb.cpp -c

test_nb: test_nb.o naivebayes.o util.o
	clang++ $(LFLAGS) naivebayes.o test_nb.o util.o -o test_nb

serializetest: serializetest.o featurizer.o
	clang++ $(LFLAGS) serializetest.o featurizer.o -o serializetest

serializetest.o: serializetest.cpp featurizer.h
	clang++ $(CFLAGS) serializetest.cpp -c

test_nb.o: test_nb.cpp
	clang++ $(CFLAGS) test_nb.cpp -c

test.o: test.cpp
	clang++ $(CFLAGS) test.cpp -c

test_ap: test_ap.o perceptron.o segmentor.o featurizer.o util.o
	clang++ $(LFLAGS) segmentor.o perceptron.o featurizer.o util.o test_ap.o -o test_ap

test_ap.o: test_ap.cpp
	clang++ $(CFLAGS) test_ap.cpp -c

perceptron.o: perceptron.cpp perceptron.h
	clang++ $(CFLAGS) perceptron.cpp -c

segmentor.o: perceptron.h segmentor.cpp segmentor.h featurizer.h
	clang++ $(CFLAGS) segmentor.cpp -c

featurizer.o: featurizer.h featurizer.cpp
	clang++ $(CFLAGS) featurizer.cpp -c

naivebayes.o: naivebayes.h naivebayes.cpp
	clang++ $(CFLAGS) naivebayes.cpp -c

util.o: util.h util.cpp
	clang++ $(CFLAGS) util.cpp -c

clean:
	rm -f *.o *.ghc test_nb train_ap test_ap serializetest train_nb train test threadtrain threadtest
