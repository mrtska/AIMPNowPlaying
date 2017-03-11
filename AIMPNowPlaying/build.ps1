Param([string] $dest, [string] $target)

New-Item -ItemType Directory -Force -Path $dest
Copy-Item -Path $target -Destination $dest
Compress-Archive -Force -Path $dest -DestinationPath  "AIMPNowPlaying.zip"
