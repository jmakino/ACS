<?xml version="1.0" encoding="utf-8"?>
<!-- $Id: rdoc.xsl,v 1.10 2002/04/21 20:57:47 wobblini Exp $     -->
<!-- $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

  <xml:output type="xml"/>

  <xsl:strip-space elements="*"/>

  <xsl:include href="files.xsl"/>
  <xsl:include href="modules.xsl"/>
  <xsl:include href="styles.xsl"/>
  <xsl:include href="tables.xsl"/>
  <xsl:include href="source.xsl"/>
  <xsl:include href="fcm.xsl"/>
  <xsl:include href="lists.xsl"/>
  <xsl:include href="labeled-lists.xsl"/>
  <xsl:include href="utils.xsl"/>

  <!-- Commented out pending decisions on parameterization
  <xsl:variable name="tiny-size">8pt</xsl:variable>
  <xsl:variable name="smaller-size">9pt</xsl:variable>
  <xsl:variable name="small-size">10pt</xsl:variable>
  <xsl:variable name="normal-size">11pt</xsl:variable>
  <xsl:variable name="large-size">12pt</xsl:variable>
  <xsl:variable name="larger-size">14pt</xsl:variable>
  <xsl:variable name="huge-size">18pt</xsl:variable>
  -->

  <xsl:key name="id" match="file|source-code-listing|Class|Module" use="@id"/>
  <xsl:template match="/">
    <fo:root>
      <fo:layout-master-set>
        <fo:simple-page-master master-name="rdocpage"
          page-width="8.5in"   page-height="11in"
          margin-top="1in"     margin-bottom="1in"
          margin-left=".75in"  margin-right=".75in">
          <fo:region-body/>
        </fo:simple-page-master>
      </fo:layout-master-set>
      <fo:page-sequence master-reference="rdocpage">
        <fo:flow font-family="serif" font-size="11pt"
          flow-name="xsl-region-body">
          <xsl:apply-templates/>
        </fo:flow>
      </fo:page-sequence>
    </fo:root>
  </xsl:template>

  <xsl:template match="rdoc">
    <fo:block font-family="Helvetica" text-align="center">
      <fo:block font-size="18pt" space-after="96pt">
        <xsl:text>RDoc Documentation</xsl:text>
      </fo:block>
      <fo:block font-size="14pt" space-after="24pt">
        <xsl:text>Includes: Files, Classes, Modules</xsl:text>
      </fo:block>
      <fo:block font-size="12pt">
        <xsl:text>(Source code listings follow descriptive listings </xsl:text>
        <xsl:text>for Class and Modules)</xsl:text>
      </fo:block>
    </fo:block>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="description//a">
    <fo:basic-link color="purple">
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates/>
    </fo:basic-link>
  </xsl:template>
</xsl:stylesheet>
