#!/usr/bin/python

import cgi, os
import cgitb; cgitb.enable()

# permet de recevoir la request de l'user
form = cgi.FieldStorage()

# on recupere le nom du fichier sense etre uploade
f = form['nude']

# on regarde si le fichier a bien ete uploade
if f.filename:
   # on supprimer le chemin d'accès principal
   # du nom de fichier pour éviter les problemes
   fn = os.path.abspath(f.filename)
   # fn = os.path.basename(f.filename)
   # message = fn
   # wb = on ecrit et on le transforme en binaire
   open(fn, 'wb').write(f.file.read())
   message = 'The file "' + fn + '" was uploaded successfully'
else:
   message = 'Pas de nude :( Sadly!'

print ("""Content-Type: text/html

<html>
<body>
   <p>%s</p>
</body>
</html>
""" % (message))

