<?xml version="1.0" encoding="utf-8"?>
<!-- $Id: convert.xsl,v 1.3 2002/03/20 23:42:42 wobblini Exp $ -->
<!-- $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:fo="http://www.w3.org/1999/XSL/Format"
                version="1.0">

  <xsl:include href="utils.xsl"/>

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="*">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates/>
    </xsl:copy>
  </xsl:template>

  <!-- file: name, id -->
  <xsl:template match="file-list/file">
    <xsl:element name="file">
      <xsl:attribute name="id">
        <xsl:value-of select="generate-id()"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="file-info/path"/>
      </xsl:attribute>
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>

  <xsl:template match="file-info">
    <xsl:element name="file-info">
      <xsl:apply-templates select="*[name() != 'path']"/>
    </xsl:element>
  </xsl:template>

  <!-- Class|Module: name, id -->
  <xsl:template match="Class|Module">
    <xsl:element name="{name()}">
      <xsl:attribute name="id">
        <xsl:value-of select="generate-id()"/>
      </xsl:attribute>
      <xsl:for-each select="@*[name() != 'id']">
        <xsl:copy-of select="."/>
      </xsl:for-each>
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>

  <!-- superclass: href or name -->
  <xsl:template match="superclass|infile">
    <xsl:variable name="href">
      <xsl:call-template name="no-hash-mark">
        <xsl:with-param name="string" select="a/@href"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:copy>
      <xsl:choose>
        <xsl:when test="string-length($href)">
          <xsl:attribute name="href">
            <xsl:value-of select="generate-id(//*[@id = $href])"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="name">
            <xsl:value-of select="."/>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:copy>
  </xsl:template>
  
  <!-- required-file|included-module: href or name -->
  <xsl:template match="required-file|included-module">
    <xsl:variable name="href">
      <xsl:call-template name="no-hash-mark">
        <xsl:with-param name="string" select="@href"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:copy>
      <xsl:choose>
        <xsl:when test="string-length($href)">
          <xsl:attribute name="href">
            <xsl:value-of select="generate-id(//*[@id = $href])"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:copy-of select="@name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:copy>
  </xsl:template>
  
  <!-- method: id, source-ref, legacy attributes -->
  <xsl:template match="method">
    <xsl:element name="method">
      <xsl:attribute name="id">
        <xsl:value-of select="generate-id()"/>
      </xsl:attribute>
      <xsl:attribute name="href">
        <xsl:value-of select="generate-id(source-code-listing)"/>
      </xsl:attribute>
      <xsl:for-each select="@*[name() != 'id']">
        <xsl:copy-of select="."/>
      </xsl:for-each>
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>

  <!-- source-code-listing: id -->
  <xsl:template match="source-code-listing">
    <xsl:element name="source-code-listing">
      <xsl:attribute name="id">
        <xsl:value-of select="generate-id()"/>
      </xsl:attribute>
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>
    
  <!-- Determine type of link, for insertion later into fo:basic-link -->
  <xsl:template match="description//a">
    <xsl:variable name="href">
      <xsl:call-template name="no-hash-mark">
        <xsl:with-param name="string" select="@href"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="target">
      <xsl:value-of select="generate-id(//*[@id = $href])"/>
    </xsl:variable>
    <xsl:element name="a">
      <xsl:choose>
        <xsl:when test="string-length($target)">
          <xsl:attribute name="internal-destination">
            <xsl:value-of select="$target"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="external-destination">
            <xsl:value-of select="$href"/>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>
  
</xsl:stylesheet>
