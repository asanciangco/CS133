export BOOST_ROOT=/usr/local/cs133/boost_1_46_0
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BOOST_ROOT/lib

export ROSE_HC_ROOT=/usr/local/cs133/rose-install
export HC_HOME=/usr/local/cs133/hc-install
export CNCC_HOME=/usr/local/cs133/cncc_distrib
export JAVA_HOME=/usr
export JAVA_TOOL_OPTIONS='-Xmx128m'

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROSE_HC_ROOT/lib:$HC_HOME/lib
export PATH=$PATH:$HC_HOME/bin:$CNCC_HOME/bin
export PATH=$PATH:$ROSE_HC_ROOT/bin
