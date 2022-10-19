<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
                xmlns:aks="http://www.julien-nevo.com/ArkosTrackerSong">
  <xsl:output method="text" encoding="utf-8"/>

  <!-- =========== (optional) prefix of identifier ============= -->
  <xsl:param name="prefix" select="''"/>

  <!-- ========= (optional) output mode ('c' or 'h') =========== -->
  <xsl:param name="output_mode" select="'c'"/>

  <!-- ============== default pattern templates ================ -->
  <!-- .name = value, -->
  <xsl:template match="*">.<xsl:value-of select="local-name(.)"/> = <xsl:value-of select="."/>, </xsl:template>

  <!-- value, -->
  <xsl:template match="*" mode="value"><xsl:value-of select="."/>, </xsl:template>

  <!-- name(value), -->
  <xsl:template match="*" mode="fn"><xsl:value-of select="local-name(.)"/>(<xsl:value-of select="."/>), </xsl:template>

  <!-- =============== "end of line" character ================= -->
  <xsl:variable name="endl"><xsl:text>
</xsl:text>
  </xsl:variable>

  <!-- ===================== identifiers ======================= -->
  <!-- identifier: "${prefix}i_tables" -->
  <xsl:variable name="i_tables"><xsl:value-of select="$prefix"/>i_tables</xsl:variable>

  <!-- identifier: "${prefix}music<i>", where <i> is 1, 2, ... -->
  <xsl:template name="music">
    <xsl:value-of select="$prefix"/>music<xsl:number count="aks:subsong" format="1"/>
  </xsl:template>

  <!-- identifier: "${prefix}music<i>_speedTracks", where <i> is 1, 2, ... -->
  <xsl:template name="speedTracks"><xsl:call-template name="music"/>_speedTracks</xsl:template>

  <!-- identifier: "${prefix}music<i>_speedTrack<j>", where <i> is 1, 2, ... -->
  <xsl:template name="speedTrack">
    <xsl:call-template name="music"/>_speedTrack<xsl:value-of select="aks:number"/>
  </xsl:template>

  <!-- identifier: "${prefix}music<i>_eventTracks", where <i> is 1, 2, ... -->
  <xsl:template name="eventTracks"><xsl:call-template name="music"/>_eventTracks</xsl:template>

  <!-- identifier: "${prefix}music<i>_eventTrack<j>", where <i> is 1, 2, ... -->
  <xsl:template name="eventTrack">
    <xsl:call-template name="music"/>_eventTrack<xsl:value-of select="aks:number"/>
  </xsl:template>

  <!-- identifier: "${prefix}music<i>_tracks", where <i> is 1, 2, ... -->
  <xsl:template name="tracks"><xsl:call-template name="music"/>_tracks</xsl:template>

  <!-- identifier: "${prefix}music<i>_track<j>", where <i> is 1, 2, ... -->
  <xsl:template name="track">
    <xsl:call-template name="music"/>_track<xsl:value-of select="aks:number"/>
  </xsl:template>

  <!-- identifier: "${prefix}music<i>_patterns", where <i> is 1, 2, ... -->
  <xsl:template name="patterns"><xsl:call-template name="music"/>_patterns</xsl:template>


  <!-- ========================= template: / ============================ -->
  <xsl:template match="/">
    <xsl:choose>
      <xsl:when test="$output_mode = 'c'">
        <xsl:apply-templates select="//aks:song"/>
      </xsl:when>
      <xsl:when test="$output_mode = 'h'">
        <xsl:apply-templates select="//aks:song" mode="h"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="aks:song" mode="h">// -*- coding: utf-8-unix -*-

#include &lt;snddrv.h&gt;
#include &lt;snd_i_table.h&gt;
#include &lt;snd_sound.h&gt;

extern const struct snd_i_table * <xsl:value-of select="$i_tables"/>[];
extern const uint8_t <xsl:value-of select="$i_tables"/>_length;

<xsl:for-each select="//aks:subsong">extern const snd_Music <xsl:call-template name="music"/>;  // "<xsl:value-of select="aks:title"/>"
</xsl:for-each>

  </xsl:template>

  <!-- ===================== template: <aks:song> ======================= -->
  <xsl:template match="aks:song">// -*- coding: utf-8-unix -*-
/*
 * <xsl:apply-templates select="aks:title"/>
 * <xsl:apply-templates select="aks:author"/>
 * <xsl:apply-templates select="aks:composer"/>
 * <xsl:apply-templates select="aks:comment"/>
 * <xsl:apply-templates select="aks:creationDate"/>
 * <xsl:apply-templates select="aks:modificationDate"/>
 * ---- play list ----<xsl:for-each select="//aks:subsong">
 * `<xsl:call-template name="music"/>`: <xsl:number value="position()" format="#1. "/>"<xsl:value-of select="aks:title"/>"</xsl:for-each>
 */

#include &lt;stdbool.h&gt;
#include &lt;stdint.h&gt;

#include &lt;snd_i_table.h&gt;
#include &lt;snd_sound.h&gt;

#define ARRAY_SIZEOF(a)    (sizeof(a) / sizeof(a[0]))
#define VEC_FROM_ARRAY(a)  {.length = ARRAY_SIZEOF(a), .data = (a)}

// End of message code
#define EOM                (uint8_t)0xff

// 16bits big endian value
#define BE16(x)            (uint8_t)((x) &gt;&gt; 8), (uint8_t)((x) &amp; 0xff)

// Macros for definition of instrument data
#define V(x)               (uint8_t)((x) &lt;&lt; 1)
#define N(x, mix)          (uint8_t)(((x) &lt;&lt; 3) | ((mix) &lt;&lt; 2) | 0x01)
#define H(w, m)            (uint8_t)((((w) &amp; 7) &lt;&lt; 5) | ((m) &lt;&lt; 3) | 0x03)
#define R(r, T)            (uint8_t)(((r) &lt;&lt; 5) | ((T) &lt;&lt; 4) | 0x07)
#define Ps_P(P)            (uint8_t)0x0f, BE16(P)
#define Ps_dP(dP)          (uint8_t)0x2f, BE16(dP)
#define Ps_dA(dA)          (uint8_t)0x4f, BE16(dA)
#define Ps_dPA(dP, dA)     (uint8_t)0x6f, BE16(dP), BE16(dA)
#define Ph_P(P)            (uint8_t)0x1f, BE16(P)
#define Ph_dP(dP)          (uint8_t)0x5f, BE16(dP)
#define Ph_dA(dA)          (uint8_t)0x9f, BE16(dA)
#define Ph_dPA(dP, dA)     (uint8_t)0xdf, BE16(dP), BE16(dA)

// Macros for definition of music data
#define EOL(skip)                  (uint8_t)(0xc0 + (skip) - 1)
#define RST                        (uint8_t)(0xe0)
#define NoteOff                    (uint8_t)(0)
#define NoteOn(x, i)               (uint8_t)(x), (uint8_t)(i)
#define Legato(x)                  (uint8_t)(x), (uint8_t)(0)
#define Exprs(n)                   (uint8_t)(0x80 + (n) - 1)
#  define reset(x)                 (uint8_t)(0x00 + ((x) &gt;&gt; 8))
#  define arpeggio3Notes(x)        (uint8_t)(0x10 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xf0)
#  define arpeggio4Notes(x)        (uint8_t)(0x20 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xff)
#  define pitchUp(x)               (uint8_t)(0x30 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xff)
#  define pitchDown(x)             (uint8_t)(0x40 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xff)
#  define fastPitchUp(x)           (uint8_t)(0x50 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xff)
#  define fastPitchDown(x)         (uint8_t)(0x60 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xff)
#  define pitchGlide(x)            (uint8_t)(0x70 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xff)
#  define volume(x)                (uint8_t)(0x80 + ((x) &gt;&gt; 8))
#  define volumeIn(x)              (uint8_t)(0x90 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xff)
#  define volumeOut(x)             (uint8_t)(0xa0 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xff)
#  define forceInstrumentSpeed(x)  (uint8_t)(0xb0 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xf0)
#  define forceArpeggioSpeed(x)    (uint8_t)(0xc0 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xf0)
#  define forcePitchTableSpeed(x)  (uint8_t)(0xd0 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xf0)
#  define arpeggioTable(x)         (uint8_t)(0xe0 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xf0)
#  define pitchTable(x)            (uint8_t)(0xf0 + ((x) &gt;&gt; 8)), (uint8_t)((x) &amp; 0xf0)

static const uint8_t EMPTY_STREAM[] = {EOM};

<!-- Generates contents of instrument data tables -->
<xsl:apply-templates select="aks:fmInstruments"/>

<!-- Generates contents of each music data -->
<xsl:apply-templates select="aks:subsongs"/>
  </xsl:template>

  <!-- ===================== template: <aks:fmInstruments> ======================= -->
  <xsl:template match="aks:fmInstruments">
// =======================================================================
// Instrument data tables

<xsl:apply-templates select="aks:fmInstrument"/>

const struct snd_i_table * <xsl:value-of select="$i_tables"/>[] = {
  <xsl:apply-templates select="aks:fmInstrument" mode="ref"/>
};
const uint8_t <xsl:value-of select="$i_tables"/>_length = ARRAY_SIZEOF(<xsl:value-of select="$i_tables"/>);
</xsl:template>

  <xsl:template match="aks:fmInstrument" mode="ref">&amp;i_table<xsl:value-of select="aks:number"/>, </xsl:template>

  <xsl:template match="aks:fmInstrument">
    <xsl:variable name="i_table">i_table<xsl:value-of select="aks:number"/></xsl:variable>
    <xsl:variable name="ad_part"><xsl:value-of select="$i_table"/>_a</xsl:variable>
    <xsl:variable name="s_part"><xsl:value-of select="$i_table"/>_s</xsl:variable>
    <xsl:variable name="r_part"><xsl:value-of select="$i_table"/>_r</xsl:variable>
    <xsl:variable name="beg" select="aks:loopStartIndex + 1"/>
    <xsl:variable name="end" select="aks:endIndex + 1"/>
// -----------------------------------------------------------------------
// <xsl:value-of select="aks:number"/>. "<xsl:value-of select="aks:title"/>"
//   isLooping = <xsl:value-of select="aks:isLooping"/>;

    <xsl:choose>
      <xsl:when test="aks:isLooping = 'true'">
static const uint8_t <xsl:value-of select="$ad_part"/>[] = {
<xsl:apply-templates select="aks:fmInstrumentCell[$beg > position()]"/>
  EOM,
};

static const uint8_t <xsl:value-of select="$s_part"/>[] = {
<xsl:apply-templates select="aks:fmInstrumentCell[(position() >= $beg) and ($end >= position())]"/>
  EOM,
};
      </xsl:when>
      <xsl:otherwise>
static const uint8_t <xsl:value-of select="$ad_part"/>[] = {
<xsl:apply-templates select="aks:fmInstrumentCell[$end >= position()]"/>
  EOM,
};

static const uint8_t <xsl:value-of select="$s_part"/>[] = {
  EOM,
};
      </xsl:otherwise>
    </xsl:choose>
static const uint8_t <xsl:value-of select="$r_part"/>[] = {
<xsl:apply-templates select="aks:fmInstrumentCell[position() > $end]"/>
  EOM,
};

static const struct snd_i_table <xsl:value-of select="$i_table"/> = {
  .wait = <xsl:value-of select="aks:speed"/>,
  .ad_part = <xsl:value-of select="$ad_part"/>,
  .s_part = <xsl:value-of select="$s_part"/>,
  .r_part = <xsl:value-of select="$r_part"/>,
};
</xsl:template>

  <xsl:template match="aks:fmInstrumentCell">
    <xsl:text>  </xsl:text>
    <xsl:choose>
      <xsl:when test="aks:link = 'softOnly'">
        <xsl:call-template name="noise">
          <xsl:with-param name="mix" select="1"/>
        </xsl:call-template>
        <xsl:call-template name="softwarePitchBend"/>
        <xsl:apply-templates select="aks:volume"/>
      </xsl:when>
      <xsl:when test="aks:link = 'hardOnly'">
        <!-- TODO -->
        <xsl:call-template name="noise">
          <xsl:with-param name="mix" select="0"/>
        </xsl:call-template>
        <xsl:call-template name="ratio"/>
        <xsl:call-template name="hardwarePitchBend"/>
        <xsl:call-template name="hardwareEnvelope">
          <xsl:with-param name="modulation" select="0"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="aks:link = 'softToHard'">
        <!-- TODO -->
        <xsl:call-template name="noise">
          <xsl:with-param name="mix" select="1"/>
        </xsl:call-template>
        <xsl:call-template name="softwarePitchBend"/>
        <xsl:call-template name="ratio"/>
        <xsl:call-template name="hardwarePitchBend"/>
        <xsl:call-template name="hardwareEnvelope">
          <xsl:with-param name="modulation" select="1"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="aks:link = 'hardToSoft'">
        <!-- TODO -->
        <xsl:call-template name="noise">
          <xsl:with-param name="mix" select="1"/>
        </xsl:call-template>
        <xsl:call-template name="softwarePitchBend"/>
        <xsl:call-template name="ratio"/>
        <xsl:call-template name="hardwarePitchBend"/>
        <xsl:call-template name="hardwareEnvelope">
          <xsl:with-param name="modulation" select="2"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="aks:link = 'noSoftNoHard'">
        <!-- TODO -->
        <xsl:call-template name="noise">
          <xsl:with-param name="mix" select="0"/>
        </xsl:call-template>
        <xsl:apply-templates select="aks:volume"/>
      </xsl:when>
      <xsl:when test="aks:link = 'softAndHard'">
        <!-- TODO -->
        <xsl:call-template name="noise">
          <xsl:with-param name="mix" select="1"/>
        </xsl:call-template>
        <xsl:call-template name="softwarePitchBend"/>
        <xsl:call-template name="ratio"/>
        <xsl:call-template name="hardwarePitchBend"/>
        <xsl:call-template name="hardwareEnvelope">
          <xsl:with-param name="modulation" select="3"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
    <xsl:value-of select="$endl"/>
  </xsl:template>

  <xsl:template name="softwarePitchBend">
    <xsl:choose>
      <xsl:when test="aks:softwarePeriod != 0">
        <xsl:text>Ps_P(</xsl:text>
        <xsl:value-of select="aks:softwarePeriod"/>
        <xsl:text>), </xsl:text>
      </xsl:when>
      <xsl:when test="aks:softwareArpeggio != 0 and aks:softwarePitch != 0">
        <xsl:text>Ps_dPA(</xsl:text>
        <xsl:value-of select="aks:softwarePitch * -1"/>
        <xsl:text>, </xsl:text>
        <xsl:value-of select="aks:softwareArpeggio * 256"/>
        <xsl:text>), </xsl:text>
      </xsl:when>
      <xsl:when test="aks:softwareArpeggio != 0">
        <xsl:text>Ps_dA(</xsl:text>
        <xsl:value-of select="aks:softwareArpeggio * 256"/>
        <xsl:text>), </xsl:text>
      </xsl:when>
      <xsl:when test="aks:softwarePitch != 0">
        <xsl:text>Ps_dP(</xsl:text>
        <xsl:value-of select="aks:softwarePitch * -1"/>
        <xsl:text>), </xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="hardwarePitchBend">
    <xsl:choose>
      <xsl:when test="aks:hardwarePeriod != 0">
        <xsl:text>Ph_P(</xsl:text>
        <xsl:value-of select="aks:hardwarePeriod"/>
        <xsl:text>), </xsl:text>
      </xsl:when>
      <xsl:when test="aks:hardwareArpeggio != 0 and aks:hardwarePitch != 0">
        <xsl:text>Ph_dPA(</xsl:text>
        <xsl:value-of select="aks:hardwarePitch * -1"/>
        <xsl:text>, </xsl:text>
        <xsl:value-of select="aks:hardwareArpeggio * 256"/>
        <xsl:text>), </xsl:text>
      </xsl:when>
      <xsl:when test="aks:hardwareArpeggio != 0">
        <xsl:text>Ph_dA(</xsl:text>
        <xsl:value-of select="aks:hardwareArpeggio * 256"/>
        <xsl:text>), </xsl:text>
      </xsl:when>
      <xsl:when test="aks:hardwarePitch != 0">
        <xsl:text>Ph_dP(</xsl:text>
        <xsl:value-of select="aks:hardwarePitch * -1"/>
        <xsl:text>), </xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ratio">
    <xsl:text>R(</xsl:text>
    <xsl:value-of select="aks:ratio"/>
    <xsl:text>, </xsl:text>
    <xsl:value-of select="aks:isRetrig"/>
    <xsl:text>), </xsl:text>
  </xsl:template>

  <xsl:template name="hardwareEnvelope">
    <xsl:param name="modulation" select="0"/>
    <xsl:text>H(</xsl:text>
    <xsl:value-of select="aks:hardwareCurve"/>
    <xsl:text>, </xsl:text>
    <xsl:value-of select="$modulation"/>
    <xsl:text>), </xsl:text>
  </xsl:template>

  <xsl:template name="noise">
    <xsl:param name="mix" select="0"/>
    <xsl:if test="aks:noise > 0">
      <xsl:text>N(</xsl:text>
      <xsl:value-of select="aks:noise"/>
      <xsl:text>, </xsl:text>
      <xsl:value-of select="$mix"/>
      <xsl:text>), </xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="aks:volume">
    <xsl:text>V(</xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>), </xsl:text>
  </xsl:template>


  <!-- ===================== template: <aks:subsongs> ======================= -->
  <xsl:template match="aks:subsongs">
// =======================================================================
// Definition of music data.

<xsl:apply-templates select="aks:subsong"/>
  </xsl:template>

  <!-- template: <aks:subsong> -->
  <xsl:template match="aks:subsong">
// -----------------------------------------------------------------------
// #<xsl:value-of select="position()"/>. "<xsl:value-of select="aks:title"/>"
// as `<xsl:call-template name="music"/>`.

<xsl:apply-templates select="aks:speedTracks"/>
<xsl:apply-templates select="aks:eventTracks"/>
<xsl:apply-templates select="aks:tracks"/>
<xsl:apply-templates select="aks:patterns"/>

const snd_Music <xsl:call-template name="music"/> = {
  .replayRate   = <xsl:value-of select="aks:replayFrequency"/>,
  .speedTracks  = VEC_FROM_ARRAY(<xsl:call-template name="speedTracks"/>),
  .eventTracks  = VEC_FROM_ARRAY(<xsl:call-template name="eventTracks"/>),
  .tracks       = VEC_FROM_ARRAY(<xsl:call-template name="tracks"/>),
  .patterns     = VEC_FROM_ARRAY(<xsl:call-template name="patterns"/>),
  .initialSpeed = <xsl:value-of select="aks:initialSpeed - 1"/>,
  .isLoop       = <xsl:value-of select="aks:endIndex > aks:loopStartIndex"/>,
  .loopToIndex  = <xsl:value-of select="aks:loopStartIndex"/>,
};
</xsl:template>

  <!-- ===================== template: <aks:speedTracks> ======================= -->
  <xsl:template match="//aks:speedTracks">
    <xsl:apply-templates select="aks:speedTrack"/>
static const snd_SpeedTrack <xsl:call-template name="speedTracks"/>[] = {
<xsl:for-each select="aks:speedTrack">  VEC_FROM_ARRAY(<xsl:call-template name="speedTrack"/>),
</xsl:for-each>
};
</xsl:template>

  <xsl:template match="//aks:speedTrack">
    <xsl:choose>
      <xsl:when test="count(aks:speedCell) = 0">
#define <xsl:call-template name="speedTrack"/>  EMPTY_STREAM
</xsl:when>
<xsl:otherwise>
static const uint8_t <xsl:call-template name="speedTrack"/>[] = {
<xsl:apply-templates select="aks:speedCell"/>
  EOM, 
};
</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="//aks:speedCell">
    <xsl:text>  </xsl:text>
    <xsl:if test="position() = 1 and aks:index != 0">EOL(<xsl:value-of select="aks:index"/>), // 0
</xsl:if>
    <!-- <xsl:apply-templates select="aks:value" mode="value"/> -->
    <xsl:value-of select="aks:value - 1"/><xsl:text>, </xsl:text>
    <xsl:call-template name="speedSkipLines"/>
    <xsl:text> // </xsl:text><xsl:value-of select="aks:index"/><xsl:value-of select="$endl"/>
  </xsl:template>

  <xsl:template name="speedSkipLines">
    <xsl:text>EOL(</xsl:text>
    <xsl:choose>
      <xsl:when test="position()=last()">
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="following-sibling::aks:speedCell[position()=1]/aks:index - aks:index"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:text>), </xsl:text>
  </xsl:template>

  <!-- ===================== template: <aks:eventTracks> ======================= -->
  <xsl:template match="//aks:eventTracks">
    <xsl:apply-templates select="aks:eventTrack"/>
static const snd_EventTrack <xsl:call-template name="eventTracks"/>[] = {
<xsl:for-each select="aks:eventTrack">  VEC_FROM_ARRAY(<xsl:call-template name="eventTrack"/>),
</xsl:for-each>
};
</xsl:template>

  <xsl:template match="//aks:eventTrack">
    <xsl:choose>
      <xsl:when test="count(aks:eventCell) = 0">
#define <xsl:call-template name="eventTrack"/>  EMPTY_STREAM
</xsl:when>
<xsl:otherwise>
static const uint8_t <xsl:call-template name="eventTrack"/>[] = {
<xsl:apply-templates select="aks:eventCell"/>
  EOM,
};
</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="//aks:eventCell">
    <xsl:text>  </xsl:text>
    <xsl:if test="position() = 1 and aks:index != 0">EOL(<xsl:value-of select="aks:index"/>), // 0
</xsl:if>
    <xsl:apply-templates select="aks:value" mode="value"/>
    <xsl:call-template name="eventSkipLines"/>
    <xsl:text> // </xsl:text><xsl:value-of select="aks:index"/><xsl:value-of select="$endl"/>
  </xsl:template>

  <xsl:template name="eventSkipLines">
    <xsl:text>EOL(</xsl:text>
    <xsl:choose>
      <xsl:when test="position()=last()">
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="following-sibling::aks:eventCell[position()=1]/aks:index - aks:index"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:text>), </xsl:text>
  </xsl:template>

  <!-- ===================== template: <aks:tracks> ======================= -->
  <xsl:template match="//aks:tracks">
    <xsl:apply-templates select="aks:track"/>
static const snd_Track <xsl:call-template name="tracks"/>[] = {
<xsl:for-each select="aks:track">  VEC_FROM_ARRAY(<xsl:call-template name="track"/>),
</xsl:for-each>
};
</xsl:template>

  <xsl:template match="//aks:track">
    <xsl:choose>
      <xsl:when test="count(aks:cell) = 0">
#define <xsl:call-template name="track"/>  EMPTY_STREAM
</xsl:when>
<xsl:otherwise>
static const uint8_t <xsl:call-template name="track"/>[] = {
<xsl:apply-templates select="aks:cell"/>
  EOM,
};
</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="//aks:cell">
    <xsl:if test="position() = 1 and aks:index != 0">  EOL(<xsl:value-of select="aks:index"/>), // 0
</xsl:if>
    <xsl:text>  </xsl:text>
    <xsl:call-template name="effects"/>
    <xsl:call-template name="note"/>
    <xsl:call-template name="skipLines"/>
    <xsl:text> // </xsl:text><xsl:value-of select="aks:index"/><xsl:value-of select="$endl"/>
  </xsl:template>

  <xsl:template name="note">
    <xsl:choose>
      <xsl:when test="aks:instrument = 0">RST, </xsl:when>
      <xsl:when test="aks:instrument > 0">NoteOn(<xsl:value-of select="aks:note + 12"/>, <xsl:value-of select="aks:instrument"/>), </xsl:when>
      <xsl:when test="aks:note">Legato(<xsl:value-of select="aks:note + 12"/>), </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="skipLines">
    <xsl:text>EOL(</xsl:text>
    <xsl:choose>
      <xsl:when test="position()=last()">
        <xsl:value-of select="1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="following-sibling::aks:cell[position()=1]/aks:index - aks:index"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:text>), </xsl:text>
  </xsl:template>

  <xsl:template name="effects">
    <xsl:if test="count(aks:effectAndValue) > 0">
      <xsl:text>Exprs(</xsl:text>
      <xsl:value-of select="count(aks:effectAndValue)"/>
      <xsl:text>), </xsl:text>
      <xsl:apply-templates select="aks:effectAndValue"/>
    </xsl:if>
  </xsl:template>

  <xsl:template match="//aks:effectAndValue">
    <xsl:value-of select="aks:effect"/>(0x<xsl:value-of select="substring(aks:hexValue, 2)"/>),
  </xsl:template>

  <!-- ===================== template: <aks:patterns> ======================= -->
  <xsl:template match="//aks:patterns">
static const snd_Pattern <xsl:call-template name="patterns"/>[] = {<xsl:apply-templates select="aks:pattern"/>
};
</xsl:template>

  <xsl:template match="//aks:pattern">
  {
    .height = <xsl:value-of select="aks:height"/>,
    .specialChannels = {
      .speedTrack = <xsl:value-of select="aks:speedTrackNumber"/>,
      .eventTrack = <xsl:value-of select="aks:eventTrackNumber"/>
    },
    .channels = {<xsl:apply-templates select="aks:patternCell"/>
    },
  },
</xsl:template>

  <xsl:template match="aks:patternCell">
      {
        .track = <xsl:value-of select="aks:trackNumber"/>,
        .detune = <xsl:value-of select="aks:transposition * 256"/>
      }, </xsl:template>

</xsl:stylesheet>
