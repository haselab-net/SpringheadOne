rem SpringheadˆÈ‰º‚ðVSS‚©‚çŽæ“¾‚·‚é
rem SVN‚Ö‚ÌˆÚs‚É”º‚Á‚ÄŽg—p‚³‚ê‚È‚­‚È‚è‚Ü‚· jumius

cd ..
ss Decloak $/Project/Springhead/src/boost
ss Decloak $/Project/Springhead/src/boost_libs
ss Workfold $/Project/Springhead .
ss Get $/Project/Springhead -R > test\log\GetVSS.log
cd test
