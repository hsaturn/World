org=$PWD
echo "Compiling SaturnLib"
cd ..
git clone https://github.com/hsaturn/SaturnLib.git
cd SaturnLib && mkdir -p build
cd build && cmake .. && make
echo "Please Enter sudo password to install SaturnLib"
sudo make install
echo "Compiling World"
cd "$org"
mkdir -p build
cd build
cmake ..
make
echo "world has been compiled in ./build"

