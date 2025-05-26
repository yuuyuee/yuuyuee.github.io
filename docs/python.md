# Python

## Install

```bash
mkdir -p ~/miniconda3
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O ~/miniconda3/miniconda.sh
bash ~/miniconda3/miniconda.sh -b -u -p ~/miniconda3
source ~/miniconda3/bin/activate
# Initialize conda
conda init --all
```

## Create environments

```bash
# Create environment named as py_env_name
conda create -n py_env_name python=3.8

# Activate the environment
conda activate py_env_name

# Install additional packages
conda install numpy

# Deactivate the environment
conda deactivate
```

## Modify pip sources

```bash
conda activate py_env_name
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
pip config set global.trusted-hosts pypi.tuna.tsinghua.edu.cn
```