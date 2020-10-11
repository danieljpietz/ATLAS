<!-- #######  YAY, I AM THE SOURCE EDITOR! #########-->
<h1 style="color: #5e9ca0;"><span style="color: #008000;">PhaseSpace MoCap Recorder</span></h1>
<h2 style="color: #2e6c80;"><span style="color: #008000;">Usage:</span></h2>
<p><em>python recorder.py </em><span style="color: #0000ff;"><em>MarkerCount BodyCount&nbsp;</em></span><span style="color: #339966;"><em>SessionName</em></span></p>
<p><span style="color: #0000ff;"><em>MarkerCount:&nbsp;</em></span>Maximum Number of markers to record</p>
<p><span style="color: #0000ff;"><em>BodyCount:&nbsp;</em></span>Maximum Number of bodies to record</p>
<p><span style="color: #339966;"><em>SessionName:&nbsp;</em></span>Optional Name to append to output</p>
<hr />
<p>Enter&nbsp;<em>kill</em> in a recording session to stop current recording</p>
<h2 style="color: #2e6c80;"><span style="color: #008000;">Output:</span></h2>
<p>The software will make a&nbsp;<em>Sessions</em> directory, if such a directory does not exists. In this directory, another directory will be made containing the name of the individual recording session. Sessions are names&nbsp;<em>YYYY-MM-DD HH MM SS.MS <span style="color: #00ff00;"><span style="color: #339966;">OptionalSessionName</span><span style="color: #000000;">.</span>&nbsp;</span></em><span style="color: #00ff00;"><span style="color: #000000;">In this directory, csv files will be created for each Marker and Body, names&nbsp;<em>Marker ##.csv&nbsp;</em>and&nbsp;<em>Body ##.csv</em>. Data in these csv's will stored with a seperate row for each frame and formatted according to the following tables:</span></span></p>
<p><span style="color: #008000;"><strong>Marker.csv</strong></span></p>
<table style="height: 23px;" width="552">
<tbody>
<tr>
<td style="width: 131px; text-align: center;">Frame time(ms)</td>
<td style="width: 131px; text-align: center;"><em>x</em>&nbsp;position</td>
<td style="width: 131px; text-align: center;"><em>y</em>&nbsp;position</td>
<td style="width: 131px; text-align: center;"><em>z</em>&nbsp;position</td>
</tr>
</tbody>
</table>
<p><span style="color: #008000;"><strong>Body.csv</strong></span></p>
<table style="height: 5px;" width="653">
<tbody>
<tr>
<td style="width: 75px; text-align: center;">Frame time(ms)</td>
<td style="width: 75px; text-align: center;"><em>x</em>&nbsp;position</td>
<td style="width: 75px; text-align: center;"><em>y</em>&nbsp;position</td>
<td style="width: 75px; text-align: center;"><em>z</em>&nbsp;position</td>
<td style="width: 75px; text-align: center;"><em>q0</em></td>
<td style="width: 75px; text-align: center;"><em>q1</em></td>
<td style="width: 75px; text-align: center;"><em>q2</em></td>
<td style="width: 76px; text-align: center;"><em>q3</em></td>
</tr>
</tbody>
</table>
