<?xml version="1.0" encoding="utf-8"?>
<!--  $Id: source.xsl,v 1.7 2002/03/20 23:42:42 wobblini Exp $ -->
<!--  $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

  <xsl:template name="source-code">
    <xsl:param name="method-list"/>
    <xsl:param name="label"/>
    <fo:block break-before="page">
      <fo:block font-size="12pt" font-family="Helvetica" space-after="24pt">
        <xsl:text>Source code for </xsl:text><xsl:value-of select="$label"/>
      </fo:block>
      <xsl:for-each select="$method-list/method">
         <xsl:sort select="(ancestor::Class|ancestor::Module)/@name"/>
         <xsl:sort select="@name"/>
        <xsl:variable name="class-or-mod" 
          select="ancestor::Class|ancestor::Module"/>
        <fo:block space-after="6pt" font-family="monospace">
          <xsl:call-template name="hr"/>
          <fo:inline>
            <xsl:value-of select="$class-or-mod/@name"/>
          </fo:inline>
          <xsl:choose>
            <xsl:when test="@category='Instance'">
              <xsl:text>#</xsl:text>
            </xsl:when>
            <xsl:when test="@category='Class'">
              <xsl:text>::</xsl:text>
            </xsl:when>
          </xsl:choose>
          <fo:inline font-weight="bold">
            <xsl:value-of select="@name"/>
          </fo:inline>
        </fo:block>
        <fo:block start-indent="9pt" end-indent="12pt" font-size="9pt" 
          id="{source-code-listing/@id}">
          <xsl:apply-templates select="source-code-listing"/>
        </fo:block>
      </xsl:for-each>
    </fo:block>
  </xsl:template>
  
  <xsl:template match="source-code-listing/span">
    <xsl:choose>
      <xsl:when test="@class='cmt'">
        <fo:inline font-style="italic"><xsl:apply-templates/></fo:inline>
      </xsl:when>
      <xsl:when test="@class='kw'">
        <fo:inline color="#3333FF" font-weight="bold">
          <xsl:apply-templates/>
        </fo:inline>
      </xsl:when>
      <xsl:when test="@class='re'">
        <fo:inline color="#662222"><xsl:apply-templates/></fo:inline>
      </xsl:when>
      <xsl:when test="@class='str'">
        <fo:inline color="#662222" 
        font-style="italic"><xsl:apply-templates/></fo:inline>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
