#!/usr/bin/perl
use strict;
use threads;
use warnings;
use POSIX;
use List::Util qw[min];
use Time::HiRes qw( gettimeofday tv_interval);
use vars qw($t0);
#use dignostics;
$t0 = [gettimeofday];

sub statusBar
{
   #$| = 1; #turn of buffering to STDOUT
   select((select(STDOUT), $|=1)[0]);
   my $currentValue = $_[0];
   my $maxValue = $_[1];
   my $segments = 50.0;
   my $barPercent = 100.0*(($maxValue/$segments)/$maxValue);
   my $percentComplete = 100.0*($currentValue/$maxValue);
   my $numberOfBars = 1; 
   my $barTotal = 0;
   while ($barTotal <= $percentComplete)
   {
      $barTotal = $numberOfBars*$barPercent;
      ++$numberOfBars;
   }
   --$numberOfBars;
   my $theBars;
   while($numberOfBars > 0)
   {
      $theBars .= "=";
      --$numberOfBars;
   }

   my $elapsedTime = tv_interval($t0, [gettimeofday]);
   $elapsedTime = $elapsedTime/60.0;
   if($percentComplete == 0)
   {
      printf("|%-50s|%5.1f%% Complete | %7.2f min.", $theBars,
            $percentComplete, $elapsedTime);
   }
   else
   {
      printf("\r|%-50s|%5.1f%% Complete | %7.2f min.", $theBars,
            $percentComplete, $elapsedTime);
   }
   if(($currentValue + 1) == $maxValue)
   {
      printf("\n ... Simulation Complete Waitng for hadd and other threads ... \n");
   }
}

sub repeatRun {
    my $iters = $_[0];
    my $id_num = $_[1];
    my $macro_name = $_[2];
    my $dir = $_[3];

    my $tempFile = "out$id_num.root";
    my $finalFile = "sum$id_num.root";
    my $i = 0;
    while ($i < $iters) {
        if ($id_num == 0) {
            #print "Thread $id_num on call $i\n";
            statusBar($i, $iters);
        }
        `./K37 $macro_name`;
        if ($i == 0) {
            `mv $dir/$tempFile $dir/$finalFile`;
        } else {
            `mv $dir/$finalFile $dir/temp$id_num.root`;
            `hadd $dir/$finalFile $dir/$tempFile $dir/temp$id_num.root 2> log.txt`;
        }
        $i++;
    }
}

sub runOnce {
#    print "@_\n";
    my $id_num = $_[0];
    my $events = $_[1];
    my @threshold = @_[2..4];
    my $e_field = $_[5];
    my $pol = $_[6];
    my $ali = $_[7];
    my $rec = $_[8];
    my @center = @_[9..11];
    my @size = @_[12..14];
    my @temp = @_[15..17];
    my $dir = $_[18];
    my $fna = $_[19];
    my @particle = @_[20..22];
    my $secondaries = $_[23];
    my $strips = $_[24];
    my $events_per_run = min($events, 1000);
    my $iters = ceil($events / $events_per_run);
    if ($id_num == 0) {
        print "$events_per_run per call\n";
        print "$iters calls\n";
    }


    open(MACRO, ">runFiles/runParallel$id_num.mac") || die;
    print MACRO "/K37/EventControls/ThresholdElectronMCP $threshold[0] keV\n";
    print MACRO
        "/K37/EventControls/ThresholdUpperScintillator $threshold[1] keV\n";
    print MACRO
        "/K37/EventControls/ThresholdLowerScintillator $threshold[2] keV\n";
    print MACRO "/K37/field/setFieldY $e_field V/m\n";
    print MACRO "/K37/gun/setPolarization $pol\n";
    print MACRO "/K37/gun/setAlignment $ali\n";
    print MACRO "/K37/gun/setTemperatureV @temp K\n";
    print MACRO "/K37/gun/setCloudSizeV @size mm\n";
    print MACRO "/K37/gun/setCloudCenter @center mm\n";
    print MACRO "/K37/gun/setRecoilCharge $rec\n";
    print MACRO "/K37/gun/setMakeBeta $particle[0]\n";
    print MACRO "/K37/gun/setMakeRecoil $particle[1]\n";
    print MACRO "/K37/gun/setMakeShakeoffElectrons $particle[2]\n";
    print MACRO "/K37/Stacking/setTrackSecondaries $secondaries\n";
    print MACRO "/K37/RunControls/fillAllSDData $strips\n";
    print MACRO "/K37/RunControls/setConfigurationFile ";
    print MACRO "runFiles/parallelConfig$id_num.txt\n";
    print MACRO "/K37/RunControls/setOutputDirectory runFiles\n";
    print MACRO "/K37/RunControls/setFilename out$id_num\n";
    print MACRO "/run/beamOn $events_per_run\n";

    close MACRO;
    repeatRun($iters, $id_num, "runFiles/runParallel$id_num.mac", "runFiles");
#   `./K37 runFiles/runParallel$id_num.mac`;
    print "Thread $id_num done\n";
    `mv runFiles/sum$id_num.root runFiles/$fna$id_num.root`;
}

#Default values in case of no parameter file
my @threshold = (0, 100, 100); #eMCP, lower scint, upper scint (keV)
my $e_field  = 35000;          #V/m uniform
my $polarization = 1.0;
my $alignment = 1.0;
my @temperature = (0.0029, 0.0029, 0.0014); #deg K
my @size = (0.51, 0.51, 0.64);              #mm
my @center = (1.07, 1.07, -2.05);           #mm
my $recoil_charge = -2;                    #-2 = mixed 
my @input;
my $directory = `pwd`; chomp($directory);
my $file_name = "output";
my @particle = ("true", "true", "true");
my $secondaries = "true";
my $strips = "true";

if (open (PARAMSin, "runFiles/params.txt")) {
    chomp($threshold[0] = <PARAMSin>);
    chomp($threshold[1] = <PARAMSin>);
    chomp($threshold[2] = <PARAMSin>);
    chomp($e_field = <PARAMSin>);
    chomp($polarization = <PARAMSin>);
    chomp($alignment = <PARAMSin>);
    chomp($recoil_charge = <PARAMSin>);
    chomp($center[0] = <PARAMSin>);
    chomp($center[1] = <PARAMSin>);
    chomp($center[2] = <PARAMSin>);
    chomp($size[0] = <PARAMSin>);
    chomp($size[1] = <PARAMSin>);
    chomp($size[2] = <PARAMSin>);
    chomp($temperature[0] = <PARAMSin>);
    chomp($temperature[1] = <PARAMSin>);
    chomp($temperature[2] = <PARAMSin>);
    chomp($directory = <PARAMSin>);
    chomp($file_name = <PARAMSin>);
    chomp($particle[0] = <PARAMSin>);
    chomp($particle[1] = <PARAMSin>);
    chomp($particle[2] = <PARAMSin>);
    chomp($secondaries = <PARAMSin>);
    chomp($strips = <PARAMSin>);
} else {
    print "\nNo parameter file...starting with default values\n\n";
}
close PARAMSin;

my $choice = -1;
while ($choice  != 0) {
    print "[1] Electron MCP threshold          = $threshold[0] keV\n";
    print "[2] Uppser Scintillator threshold   = $threshold[1] keV\n";
    print "[3] Electron MCP threshold          = $threshold[2] keV\n";
    print "Enter number to change or 0 to quit\n";
    chomp($choice = <STDIN>);
    if ($choice != 0) {
        printf "Enter new value for $choice\n";
        chomp($threshold[$choice-1] = <STDIN>);
    }
}

$choice = -1;
while ($choice  != 0) {
    print "[1] Electric field                 = $e_field V/m \n";
    print "[2] Polarization                   = $polarization\n";
    print "[3] Alignment                      = $alignment\n";
    print "[4] Recoil charge                  = $recoil_charge\n";
    print "                                     (-2 for mixed)\n";
    print "                                     (-3 for photoions)\n";
    print "Enter number to change or 0 to quit\n";
    chomp($choice = <STDIN>);
    if ($choice != 0) {
        printf "Enter new value for $choice\n";
        if ($choice == 1) { chop($e_field = <STDIN>); }
        if ($choice == 2) { chop($polarization = <STDIN>); }
        if ($choice == 3) { chop($alignment = <STDIN>); }
        if ($choice == 4) { chop($recoil_charge = <STDIN>); }
    }
}

$choice = -1;
while ($choice  != 0) {
    print "[1] Cloud position        x         = $center[0] mm\n";
    print "[2]                       y         = $center[1] mm\n";
    print "[3]                       z         = $center[2] mm\n";
    print "[4] Cloud size            x         = $size[0] mm\n";
    print "[5]                       y         = $size[1] mm\n";
    print "[6]                       z         = $size[2] mm\n";
    print "[7] Cloud temperature     x         = $temperature[0] K\n";
    print "[8]                       y         = $temperature[1] K\n";
    print "[9]                       z         = $temperature[2] K\n";
    print "Enter number to change or 0 to quit\n";
    chomp($choice = <STDIN>);
    if ($choice != 0) {
        printf "Enter new value for $choice\n";
        if ($choice <  4) { chomp($center[$choice-1] = <STDIN>); }
        if ($choice >= 4 && $choice < 7) { chomp($size[$choice-4] = <STDIN>); }
        if ($choice >= 7) { chomp($temperature[$choice-7] = <STDIN>); }
    }
}

$choice = -1;
while($choice != 0) {
    print "[1] Make Beta                          = $particle[0]\n";
    print "[2] Make Recoils                       = $particle[1]\n";
    print "[3] Make Shakeoff Electrons            = $particle[2]\n";
    print "[4] Track secondaries                  = $secondaries\n";
    print "[5] Record DSSD Data                   = $strips\n";
    print "Enter number to change or 0 to quit\n";
    chomp($choice = <STDIN>);
    if ($choice != 0) {
        printf "Enter new value for $choice\n";
        if ($choice <= 3) {
            chomp($particle[$choice-1] = <STDIN>);
        } elsif ($choice == 4) {
            chomp($secondaries = <STDIN>);
        } elsif ($choice == 5) {
            chomp($strips = <STDIN>);
        } else {
            print "Unknown value.  Try again\n";
        }
    }
}

$choice = -1;
my $ovw = "n";
while ($choice  != 0) {
    print "[1] Output directory                     = $directory\n";
    print "[2] Output file name (.root automatic)   = $file_name\n";
    print "Enter number to change or 0 to quit\n";
    chomp($choice = <STDIN>);
    if ($choice != 0) {
        printf "Enter new value for $choice\n";
        if ($choice == 1) {chomp($directory = <STDIN>);}
        if ($choice == 2) {chomp($file_name = <STDIN>);}
    }
    $directory=~ s/ //g;
    $file_name=~ s/ //g;
    if (-e "$directory/$file_name.root") {
        print "File $directory/$file_name already exists.\n";
        print "Do you really want to overwrite this file (y/n)?\n";
        chomp($ovw = <STDIN>);
        if ($ovw eq "n" || $ovw eq "N") {
#            $choice = -1;
        }

    } else {
        print "File $directory/$file_name does not exist\n";
    }
}

#Remove white spaces
$directory =~ s/\s+//g;
$file_name =~ s/\s+//g;
my $proc = 1;
chomp(my $os = `uname`);
printf("OS: $os\n");
if ($os eq "Linux") {
    chomp($proc = `nproc`);
} elsif ($os eq "Darwin") {
    chomp($proc = `sysctl -n hw.ncpu`);
} else {
    printf("Unknown operating system... assuming $proc processors\n");
}

print "You have $proc processors...how many do you want to use?\n";
chomp($choice = <STDIN>);
if ($choice <= $proc && $choice > 0) {
    $proc = $choice;
} else {
    $proc--
}


print "Enter total number of events to simulate\n";
chomp(my $events_total = <STDIN>);
my $thread_events = ceil($events_total / $proc);

print "Running $events_total events on $proc processors\n";
print "$thread_events per thread\n";

my @allParams = (@threshold, $e_field, $polarization, $alignment,
                 $recoil_charge, @center, @size, @temperature, $directory,
                 $file_name, @particle, $secondaries, $strips);
unshift(@allParams, 0, $thread_events);     #  Add to the front of allParams

my @jobs;
my $i = 0;
while ($i < $proc) {
    $allParams[0] = $i;
    # push @jobs, threads->create(sub {
    #     print  "./K37 runFiles/runParallel$i.mac\n";
    #     my $res=`./K37 runFiles/runParallel$i.mac`;
    #     my $fnamed = "runFiles/output$i.txt";
    #     open my $OUTPUT, '>', $fnamed or
    #         die "Couldn't open output.txt: $!\n";
    #     print $OUTPUT $res;
    #     close $OUTPUT
    #                             });
    push @jobs, threads -> create(\&runOnce, @allParams);
    $i++;
}
$_ -> join for @jobs;
$i = 0;
my @tempfiles;
while ($i < $proc) {
    push @tempfiles, "runFiles/$file_name$i.root ";
    $i++;
}
if (-e "$directory/$file_name.root" && ($ovw eq "n" || $ovw eq "N")) {
    print "WARNING File $directory/$file_name.root has been created recently\n";
    my $z = 0;
    while (-e "$directory/$file_name$z.root") {
        $z++;
    }
    print "Moving old file to $directory/$file_name$z.root\n";
    `mv $directory/$file_name.root $directory/$file_name$z.root`;
}
`hadd -f $directory/$file_name.root @tempfiles 2>log.txt`;

# Print the most recently used parameters to a file
open(PARAMS, ">runFiles/params.txt") ||
    die "Couldn't open make runFiles directory\n";
foreach(@allParams[2..$#allParams]) { # skip the zeroeth element
    $_=~ s/ //g;                      # Remove any whitespace
    print PARAMS "$_\n";
}

close PARAMS;
