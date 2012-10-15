
SRCE_HOME="/home/ms6401/projects/Code_Sandbox/geographic"
DEST_HOME="/home/ms6401/projects/Medusa"


# Geo Notes Files
mkdir -p ${DEST_HOME}/Marvin/GeoNotes
cp -r ${SRCE_HOME}/geo_notes/*          ${DEST_HOME}/Marvin/GeoNotes
cp    ${SRCE_HOME}/geo_notes/.gitignore ${DEST_HOME}/Marvin/GeoNotes

# Geo Image
mkdir -p ${DEST_HOME}/GeoImage
cp -r ${SRCE_HOME}/geo_image/*          ${DEST_HOME}/GeoImage
cp -r ${SRCE_HOME}/geo_image/.gitignore ${DEST_HOME}/GeoImage

# Orthoproject
mkdir -p ${DEST_HOME}/OrthoProject
cp -r ${SRCE_HOME}/ortho_project/*           ${DEST_HOME}/OrthoProject
cp -r ${SRCE_HOME}/ortho_project/.gitignore  ${DEST_HOME}/OrthoProject



