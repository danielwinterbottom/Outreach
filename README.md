# Outreach
Code to produce ntuples for outreach purposes 

# Setup CMSSW workarea

```
cmsrel CMSSW_7_6_7
cd CMSSW_7_6_7/src
cmsenv
```

# Clone repository

```
git clone git@github.com:danielwinterbottom/Outreach.git
```

# Produce ZEE (ZMM) ntuples

```
cd Outreach/ZLLAnalyser/
scram b -j8
cmsRun python/ZEE_cfg.py
```
