OutFile "OM_installer.exe"

InstallDir "$DESKTOP\Ocean Madness"



#installer
Section 

    SetOutPath $INSTDIR


    File "DeploymentFolder\Ocean Madness.exe"
    File "DeploymentFolder\lights.txt"
    File "DeploymentFolder\READ ME.txt"
    File "DeploymentFolder\textures.txt"
    File "DeploymentFolder\assimp-vc142-mt.dll"
    
    File /r "DeploymentFolder\assets"


SectionEnd
