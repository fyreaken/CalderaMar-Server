# Darkflame Universe Account Manager

THIS FORK INCLUDES LEGO6245'S NAME APPROVAL DASHBOARD (https://github.com/Lego6245)

Alternative instructions: https://gist.github.com/Saymoan/cee72d3e6244a51f70124ac86a2802e9#installing-accountmanager (the git clone needs to be changed)

##########

This is a quick and simple web application intended for account creation and management for a [DLU](https://github.com/DarkflameUniverse/DarkflameServer) instance created by Wincent.

Logo by BlasterBuilder.

## Run and Install 
* Clone this repository
* Install `requirements.txt`
* Create `credentials.py`
```py
# credentials.py

# Make sure this is a long random string
SECRET_KEY = 'long-random-string'

# Replace instances of <> with the database credentials
DB_URL = 'mysql+pymysql://<mysql-user>:<mysql-password>@<mysql-host>/<mysql-database>'
```
* Create `resources.py`
```py
# resources.py

# Path to the logo image to display on the application
LOGO = 'logo/logo.png'

# Path to the privacy policy users have to agree to
PRIVACY_POLICY = 'policy/Privacy Policy.pdf'

# Path to the terms of use users have to agree to
TERMS_OF_USE = 'policy/Terms of Use.pdf'
```
* Run the application
```sh
# Run the python application, with a given port number
flask run --port 5000
# or simply
python app.py
```
