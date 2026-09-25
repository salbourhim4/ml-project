# ML Project

A feedforward neural network implemented from scratch in C, trained on MNIST handwritten digits — no external ML libraries, just a custom matrix library and manually-derived backpropagation.

## Architecture

`784 → 128 → 64 → 10`

- Two hidden layers (128 and 64 units) with sigmoid activation
- Softmax output layer
- Cross-entropy loss
- Trained with plain per-sample stochastic gradient descent (no mini-batching)

## Result

After training for 5 epochs on the 60,000-image MNIST training set, the network reaches **96.2% accuracy (9,623/10,000)** on the held-out 10,000-image test set.

## Build

```
make
```

Produces the `ml-project` binary. Requires a C compiler (`gcc`) and links against `libm`.

## Run

```
./ml-project [epochs] [learning_rate]
```

Both arguments are optional and default to `5` epochs and a learning rate of `0.1`. For example:

```
./ml-project 10 0.05
```

An invalid or non-positive value for either argument prints a warning and falls back to its default instead of failing.

The MNIST IDX files must be present, uncompressed, in a `data/` directory at the repo root:

```
data/train-images-idx3-ubyte
data/train-labels-idx1-ubyte
data/t10k-images-idx3-ubyte
data/t10k-labels-idx1-ubyte
```

These aren't included in the repo (see `.gitignore`). Download the MNIST database (Yann LeCun's original site, or any standard mirror such as Kaggle's MNIST dataset) and decompress the four `.gz` files into `data/` with the filenames above.

Running the binary trains the network from scratch and prints per-epoch average loss, periodic progress within each epoch, and final test-set accuracy. Trained weights are not saved — every run starts from a fresh random initialization.

## Design notes

- **Matrix library** (`matrix.c`/`matrix.h`): a small `Matrix` type (`rows`, `cols`, flat `float*` data) with the operations needed for forward/backward passes — add, subtract, multiply, scale, transpose, elementwise multiply — plus random initialization for weights.
- **Two-arena memory model**: all allocation goes through a bump-allocating `Arena`, not `malloc`/`free` per matrix. A single **model arena** is created once and holds everything that needs to persist for the whole run — the loaded MNIST data and the network's weights, biases, and gradients. A separate **scratch arena** is reset after every training/inference sample; it holds the per-sample intermediate matrices (`z`/`a` values in the forward pass, gradient matrices in the backward pass) that would otherwise need to be freed individually. This keeps memory management simple and avoids allocator overhead in the hot per-sample loop.
