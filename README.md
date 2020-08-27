# ROS 2 stack for external control of the ABB YuMi.
<p align="center">
  <img src="https://github.com/yumi-crew/yumi/blob/eloquent/yumi_description/meshes/yumi_render.png" width=300>
</p>

**Currently in a non functional state!** 

### Dependencies:
* vcs import
   * `sudo apt-get install python3-vcstool`

* abb_librws dependencies
   * POCO C++ Libraries (>= 1.4.3 due to WebSocket support)
       * Navigate to home dir.
         Should be built from source.
         ~~~~
         sudo apt-get -y update && sudo apt-get -y install git g++ make cmake libssl-dev  
         git clone -b master https://github.com/pocoproject/poco.git   
         cd poco
         mkdir cmake-build 
         cd cmake-build
         sudo cmake ..
         sudo cmake --build . --target install
         ~~~~
* abb_libegm dependencies
  * Boost
     * `sudo apt-get install libboost-all-dev`
  * Protobuf 
     * Should be built from source.
        ~~~~ 
        sudo apt-get install autoconf automake libtool curl make g++ unzip  
        git clone https://github.com/protocolbuffers/protobuf.git  
        cd protobuf  
        ./configure
        sudo make
        sudo make install
        sudo ldconfig # refresh shared library cache.
        ~~~~ 

* Moveit 2 dependencies
    * yaml-cpp   
        * `sudo apt-get install libyaml-cpp-dev` 
     