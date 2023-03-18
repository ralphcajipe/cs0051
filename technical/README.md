# [Technical-Summative] Summative Assessment 3 GPGPU Programming

# Downloads
NVIDIA HPC SDK <br>
https://developer.nvidia.com/nvidia-hpc-sdk-downloads

NVIDIA HPC SDK Installation <br>
https://docs.nvidia.com/hpc-sdk/hpc-sdk-install-guide/index.html#install-linux-end-usr-env-settings:~:text=In%20bash%2C%20sh%2C%20or%20ksh%2C%20use%20these%20commands%3A

<br>

# The 7 commands <br>
In bash, sh, or ksh, use these commands:<br>
NVARCH=`uname -s`_`uname -m`; export NVARCH <br>
NVCOMPILERS=/opt/nvidia/hpc_sdk; export NVCOMPILERS <br>
MANPATH=$MANPATH:$NVCOMPILERS/$NVARCH/23.1/compilers/man; >export MANPATH <br>
PATH=$NVCOMPILERS/$NVARCH/23.1/compilers/bin:$PATH; export PATH <br>

And the equivalent in bash, sh, and ksh:
export PATH=$NVCOMPILERS/$NVARCH/23.1/comm_libs/mpi/bin:$PATH
export MANPATH=$MANPATH:$NVCOMPILERS/$NVARCH/23.1/comm_libs/mpi/ma

export LD_LIBRARY_PATH=/usr/lib/wsl/lib

# Compiling
### Normal compile <br>
    gcc normal.cpp -o normal <br>

### Nvidia compile <br>
    nvc++ normal.cpp -o compiled-nvc++ <br>

#pragma acc kernels <br>
    -> line of code to optimize <br>

#pragma acc data copyin(a, sumpair), copyout(sumpair) <br>
{ <br>
#pragma acc parallel <br>
{ <br>
#pragma acc loop independent <br>
    -> for loop <br>
} <br>
}
### OpenACC compile CPU host <br>
    nvc++ -acc=host normal.cpp compiled-openacc <br>

### OpenACC compile GPU host <br>
    nvc++ -acc=gpu -gpu=cc50 notmal.cpp -o compiled-openacc-gpu

Open the Task Manager to see the spike when Intel or Nvidia is ran.