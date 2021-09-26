# Add this to .zshrc on a manjaro live usb with the source code in the same folder

echo "Setup env"
sudo pacman -Sy
sudo pacman -S make cmake gcc libpng zlib pthread allegro

echo "Compile and run"
mkdir -p bin; cd bin; cmake .. -DCMAKE_BUILD_TYPE=RELEASE && cmake --build . && ./FreshersTD_Hackathon &

echo "Return to .zshrc"
