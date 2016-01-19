#!/usr/bin/perl
#-----------------------------------------------------------------------------
#
# Copyright (C) 2011
#   Michael Theall (mtheall)
#   Dave Murphy (WinterMute)
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any
# damages arising from the use of this software.
#
# Permission is granted to anyone to use this software for any
# purpose, including commercial applications, and to alter it and
# redistribute it freely, subject to the following restrictions:
#
# 1.  The origin of this software must not be misrepresented; you
#   must not claim that you wrote the original software. If you use
#   this software in a product, an acknowledgment in the product
#   documentation would be appreciated but is not required.
# 2.  Altered source versions must be plainly marked as such, and
#   must not be misrepresented as being the original software.
# 3.  This notice may not be removed or altered from any source
#   distribution.
#
#-----------------------------------------------------------------------------
  use strict;

  my $dir = "$ENV{HOME}/devkitPro";
  my $downloader;
  my $archname;

  if($ENV{"DEVKITPRO"} ne "")
  {
    $dir = $ENV{"DEVKITPRO"};
  }

  if($#ARGV eq 0)
  {
    $dir = $ARGV[0];
  }

  # Ensure full pathname
  if(!($dir =~ /^\//))
  {
    my $pwd = `pwd`;
    chomp($pwd);
    $dir = "$pwd/$dir";
  }

  printf("devkitARM Updater/Installer\n");
  printf("Installing to %s\n", $dir);

  # Get OS information
  my $os = `uname`;
  my $arch = `uname -m`;
  chomp($os);
  chomp($arch);

  # Check OS information
  if($os eq "Linux" and ($arch eq "i686" or $arch eq "x86_64"))
  {
  $downloader = "wget -q";
  $archname = $arch . "-linux";
  }
  elsif($os eq "Darwin")
  {
  $downloader = "curl -L -O -s";
  $archname = "osx";
  }
  else
  {
    printf(STDERR "Not on Linux i686/x86_64 or Darwin!\n");
    exit(1);
  }

  # Set up directories
  if(!(-d "$dir"))
  {
    mkdir("$dir") or die $!;
  }

  if(!(-d "$dir/libctru"))
  {
    mkdir("$dir/libctru") or die $!;
  }
  if(!(-d "$dir/libnds"))
  {
    mkdir("$dir/libnds") or die $!;
  }
  if(!(-d "$dir/libgba"))
  {
    mkdir("$dir/libgba") or die $!;
  }
  if(!(-d "$dir/libmirko"))
  {
    mkdir("$dir/libmirko") or die $!;
  }
  if(!(-d "$dir/examples"))
  {
    mkdir("$dir/examples") or die $!;
  }
  if(!(-d "$dir/examples/nds"))
  {
    mkdir("$dir/examples/nds") or die $!;
  }
  if(!(-d "$dir/examples/gba"))
  {
    mkdir("$dir/examples/gba") or die $!;
  }
  if(!(-d "$dir/examples/gp32"))
  {
    mkdir("$dir/examples/gp32") or die $!;
  }
  if(!(-d "$dir/examples/3ds"))
  {
    mkdir("$dir/examples/3ds") or die $!;
  }

  # Grab update file
  if(-e "devkitProUpdate.ini")
  {
  unlink("devkitProUpdate.ini") or die $!;
  }
  printf("Downloading update file...");
  system($downloader . " http://devkitpro.sourceforge.net/devkitProUpdate.ini") and die "Failed to download!";
  printf("OK!\n");

  # Initialize versions & newVersions
  my %versions =
    (
      'devkitARM'    => 0,
      'libgba'       => 0,
      'libgbafat'    => 0,
      'maxmodgba'    => 0,
      'gbaexamples'  => 0,
      'libmirko'     => 0,
      'gp32examples' => 0,
      'libnds'       => 0,
      'libndsfat'    => 0,
      'maxmodds'     => 0,
      'dswifi'       => 0,
      'ndsexamples'  => 0,
      'defaultarm7'  => 0,
      'filesystem'   => 0,
      'libctru'      => 0,
      'citro3d'      => 0,
      '3dsexamples'  => 0,
    );
  my %newVersions = %versions;

  my %files    = ();
  my $current  = "";

  if(-e "$dir/dkarm-update.ini")
  {
    open(MYFILE, "<$dir/dkarm-update.ini") or die $!;
    while(<MYFILE>)
    {
      chomp;
      if($_ =~ /\[(.*)\]/)
      {
        $current = $1;
      }
      elsif($_ =~ /Version=(.*)/ and defined($versions{$current}))
      {
        $versions{$current} = $1;
      }
      elsif($_ =~ /File=(.*)/)
      {
        $files{$current} = $1;
      }
    }
    close(MYFILE);
  }

  my %newFiles = ();

  open(MYFILE, "<devkitProUpdate.ini") or die $!;
  while(<MYFILE>)
  {
    chomp;
    if($_ =~ /\[(.*)\]/)
    {
      $current = $1;
    }
    elsif($_ =~ /Version=(.*)/ and defined($newVersions{$current}))
    {
      $newVersions{$current} = $1;
    }
    elsif($_ =~ /File=(.*)/)
    {
      $newFiles{$current} = $1;
    }
  }
  close(MYFILE);
  unlink("devkitProUpdate.ini") or die $!;

  # see what to update
  my %updates = ();
  foreach my $key (keys %versions)
  {
    if($versions{$key} ne $newVersions{$key} and $newVersions{$key} ne 0)
    {
      $newFiles{$key} =~ s/win32\.exe/$archname.tar.bz2/;
      $updates{$key} = $newFiles{$key};
    }
    else
    {
      printf("%s is up-to-date\n", $key);
    }
  }

  # Download files
  foreach my $key (keys %updates)
  {
    printf("Update %s with %s\n", $key, $updates{$key});
    if(-e $updates{$key})
    {
      unlink($updates{$key});
    }

    my $cmd = sprintf("%s http://download.sourceforge.net/devkitpro/%s", $downloader, $updates{$key});
    printf("  Downloading...");
    system($cmd) and die "Failed to download $updates{$key}\n";
    printf("OK!\n");
  }

  # Install files
  my %install =
    (
      'devkitARM'    => '',
      'libgba'       => 'libgba',
      'libgbafat'    => 'libgba',
      'maxmodgba'    => 'libgba',
      'gbaexamples'  => 'examples/gba',
      'libmirko'     => 'libmirko',
      'gp32examples' => 'examples/gp32',
      'libnds'       => 'libnds',
      'libndsfat'    => 'libnds',
      'maxmodds'     => 'libnds',
      'dswifi'       => 'libnds',
      'ndsexamples'  => 'examples/nds',
      'defaultarm7'  => 'libnds',
      'filesystem'   => 'libnds',
      'libctru'      => 'libctru',
      'citro3d'      => 'libctru',
      '3dsexamples'  => 'examples/3ds',
    );

  foreach my $key (keys %updates)
  {
    my $cmd = sprintf("tar -xjf %s -C $dir/%s", $updates{$key}, $install{$key});
    printf("Extracting %s...", $updates{$key});
    system($cmd) and die "Failed\n";
    printf("OK!\n");
  }

  # Output update info
  open(MYFILE, ">$dir/dkarm-update.ini") or die $!;
  foreach my $key (keys %newVersions)
  {
    printf(MYFILE "[%s]\n", $key);
    printf(MYFILE "Version=%s\n", $newVersions{$key});
    printf(MYFILE "File=%s\n", $newFiles{$key});
    printf(MYFILE "\n");
  }
  close(MYFILE);

  # Check environment variables
  printf("Checking DEVKITPRO...");
  my $env = `echo \$DEVKITPRO`;
  chomp($env);
  if($env ne "$dir")
  {
    printf("Please set DEVKITPRO in your environment as $dir\n");
  }
  else
  {
    printf("OK!\n");
  }

  printf("Checking DEVKITARM...");
  $env = `echo \$DEVKITARM`;
  chomp($env);
  if($env ne "$dir/devkitARM")
  {
    printf("Please set DEVKITARM in your environment as \${DEVKITPRO}/devkitARM\n");
  }
  else
  {
    printf("OK!\n");
  }

  exit(0);
