<?xml version="1.0" encoding="utf-8"?>
<!--  $Id: styles.xsl,v 1.7 2002/03/19 00:57:22 wobblini Exp $     -->
<!--  $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

<!-- This template is based on something in a Docbook XSL file. -->
  <xsl:template match="pre|source-code-listing">
    <fo:block wrap-option="no-wrap"
      font-family="monospace"
      text-align="start"
      white-space-collapse="false"
      linefeed-treatment="preserve">
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

  <xsl:template match="b">
    <fo:inline font-weight="bold">
      <xsl:apply-templates/>
    </fo:inline>
  </xsl:template>

  <xsl:template match="em">
    <fo:inline font-weight="bold">
      <xsl:apply-templates/>
    </fo:inline>
  </xsl:template>

  <xsl:template match="path|dtm-created|dtm-modified|
                       file/@name|required-file/@name
                       |tt">
    <fo:inline font-family="monospace">
      <xsl:value-of select="."/>
    </fo:inline>
  </xsl:template>

  <xsl:template match="h1">
    <fo:block font-family="Helvetica" font-size="18pt" 
      space-before="12pt" space-after="6pt">
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

  <xsl:template match="h2">
    <fo:block font-family="Helvetica" font-size="14pt" 
      space-before="6pt" space-after="4pt">
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

  <xsl:template name="hr">
    <fo:block>
      <fo:inline>
        <fo:leader leader-length="6.5in" leader-pattern="rule"
          leader-pattern-width="1pt"/>
      </fo:inline>
    </fo:block>
  </xsl:template>

  <xsl:template match="p">
    <fo:block space-after="12pt">
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

</xsl:stylesheet>
